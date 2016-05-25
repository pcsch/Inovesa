/******************************************************************************
 * Inovesa - Inovesa Numerical Optimized Vlasov-Equation Solver Algorithms   *
 * Copyright (c) 2014-2016: Patrik Schönfeldt                                 *
 *                                                                            *
 * This file is part of Inovesa.                                              *
 * Inovesa is free software: you can redistribute it and/or modify            *
 * it under the terms of the GNU General Public License as published by       *
 * the Free Software Foundation, either version 3 of the License, or          *
 * (at your option) any later version.                                        *
 *                                                                            *
 * Inovesa is distributed in the hope that it will be useful,                 *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU General Public License for more details.                               *
 *                                                                            *
 * You should have received a copy of the GNU General Public License          *
 * along with Inovesa.  If not, see <http://www.gnu.org/licenses/>.           *
 ******************************************************************************/

#include "SM/KickMap.hpp"

vfps::KickMap::KickMap( vfps::PhaseSpace* in, vfps::PhaseSpace* out,
                        const meshindex_t xsize, const meshindex_t ysize,
                        const InterpolationType it, const bool interpol_clamp,
                        const DirectionOfKick kd) :
    SourceMap(in,out,kd==DirectionOfKick::x?1:xsize,
                       kd==DirectionOfKick::x?ysize:1,it,it),
    _kickdirection(kd),
    _meshsize_kd(kd==DirectionOfKick::x?xsize:ysize),
    _meshsize_pd(kd==DirectionOfKick::x?ysize:xsize)
{
    if (interpol_clamp && !OCLH::active) {
        Display::printText("Clamped interpolation not "
                           "implemented for KickMap w/o OpenCL.");
    }
    _offset.resize(_meshsize_pd,meshaxis_t(0));
    #ifdef INOVESA_INIT_KICKMAP
    for (meshindex_t q_i=0; q_i<_meshsize_pd; q_i++) {
        _hinfo[q_i*_ip].index = _meshysize/2;
        _hinfo[q_i*_ip].weight = 1;
        for (unsigned int j1=1; j1<_it; j1++) {
            _hinfo[q_i*_ip+j1].index = 0;
            _hinfo[q_i*_ip+j1].weight = 0;
        }
    }
    #endif
    #ifdef INOVESA_USE_CL
    if (OCLH::active) {
        _force_buf = cl::Buffer(OCLH::context,CL_MEM_READ_WRITE,
                                sizeof(meshaxis_t)*_meshsize_pd);
        _cl_code += R"(
        __kernel void apply_xKick(const __global data_t* src,
                                  const __global data_t* dx,
                                  const int meshsize,
                                  __global data_t* dst)
        {
            const int y = get_global_id(0);
            const int dxi = floor(dx[y]);
            const data_t dxf = dx[y] - dxi;
            data_t value;
            int x=0;
            while (x-1+dxi<0) {
                dst[x*meshsize+y]  = 0;
                x++;
            }
            while (x+2+dxi<meshsize && x < meshsize) {
        )";
        if (interpol_clamp) {
            _cl_code += R"(
                data_t ceil = max(src[(x  +dxi)*meshsize+y],
                                  src[(x+1+dxi)*meshsize+y]);
                data_t flor = min(src[(x  +dxi)*meshsize+y],
                                  src[(x+1+dxi)*meshsize+y]);
            )";
        }
        _cl_code += R"(
                value = mult(src[(x+dxi-1)*meshsize+y],
                            (dxf  )*(dxf-1)*(dxf-2)/(-6))
                      + mult(src[(x   +dxi)*meshsize+y],
                            (dxf+1)*(dxf-1)*(dxf-2)/( 2))
                      + mult(src[(x+1+dxi)*meshsize+y],
                            (dxf+1)*(dxf  )*(dxf-2)/(-2))
                      + mult(src[(x+2+dxi)*meshsize+y],
                            (dxf+1)*(dxf  )*(dxf-1)/( 6));
        )";
        if (interpol_clamp) {
            _cl_code += "dst[x*meshsize+y] = clamp(value,flor,ceil);";
        } else {

            _cl_code += "dst[x*meshsize+y] = value;";
        }
        _cl_code += R"(
                x++;
            }
            while (x < meshsize) {
                dst[x*meshsize+y]  = 0;
                x++;
            }
        }
        )";

        _cl_code += R"(
        __kernel void apply_yKick(const __global data_t* src,
                                  const __global data_t* dy,
                                  const int meshsize,
                                  __global data_t* dst)
        {
            const int x = get_global_id(0);
            const int meshoffs = x*meshsize;
            const int dyi = floor(dy[x]);
            const data_t dyf = dy[x] - dyi;
            data_t value;
            int y=0;
            while (y-1+dyi<0) {
                dst[meshoffs+y]  = 0;
                y++;
            }
            while (y+2+dyi<meshsize && y < meshsize) {
        )";
        if (interpol_clamp) {
            _cl_code += R"(
                data_t ceil = max(src[meshoffs+y  +dyi],
                                  src[meshoffs+y+1+dyi]);
                data_t flor = min(src[meshoffs+y  +dyi],
                                  src[meshoffs+y+1+dyi]);
            )";
        }
        _cl_code += R"(
                value = mult(src[meshoffs+y-1+dyi],
                            (dyf  )*(dyf-1)*(dyf-2)/(-6))
                      + mult(src[meshoffs+y  +dyi],
                            (dyf+1)*(dyf-1)*(dyf-2)/( 2))
                      + mult(src[meshoffs+y+1+dyi],
                            (dyf+1)*(dyf  )*(dyf-2)/(-2))
                      + mult(src[meshoffs+y+2+dyi],
                            (dyf+1)*(dyf  )*(dyf-1)/( 6));
        )";
        if (interpol_clamp) {
            _cl_code += "dst[x*meshsize+y] = clamp(value,flor,ceil);";
        } else {

            _cl_code += "dst[x*meshsize+y] = value;";
        }
        _cl_code += R"(
                dst[meshoffs+y] = value;
                y++;
            }
            while (y < meshsize) {
                dst[meshoffs+y]  = 0;
                y++;
            }
        }
        )";
        _cl_prog  = OCLH::prepareCLProg(_cl_code);

        if (_kickdirection == DirectionOfKick::x) {
            applyHM = cl::Kernel(_cl_prog, "apply_xKick");
        } else {
            applyHM = cl::Kernel(_cl_prog, "apply_yKick");
        }
        applyHM.setArg(0, _in->data_buf);
        applyHM.setArg(1, _force_buf);
        applyHM.setArg(2, _meshsize_kd);
        applyHM.setArg(3, _out->data_buf);
    }
    #endif // INOVESA_USE_CL
}

vfps::KickMap::~KickMap()
{
}

void vfps::KickMap::apply()
{
    #ifdef INOVESA_USE_CL
    if (OCLH::active) {
        #ifdef INOVESA_SYNC_CL
        _in->syncCLMem(clCopyDirection::cpu2dev);
        #endif // INOVESA_SYNC_CL
        OCLH::queue.enqueueNDRangeKernel (
                    applyHM,
                    cl::NullRange,
                    cl::NDRange(_meshsize_pd));
        #ifdef CL_VERSION_1_2
        OCLH::queue.enqueueBarrierWithWaitList();
        #else // CL_VERSION_1_2
        OCLH::queue.enqueueBarrier();
        #endif // CL_VERSION_1_2
        #ifdef INOVESA_SYNC_CL
        _out->syncCLMem(clCopyDirection::dev2cpu);
        #endif // INOVESA_SYNC_CL
    } else
    #endif // INOVESA_USE_CL
    {
    meshdata_t* data_in = _in->getData();
    meshdata_t* data_out = _out->getData();

    if (_kickdirection == DirectionOfKick::x) {
        for (meshindex_t x=0; x< static_cast<meshindex_t>(_meshsize_kd); x++) {
            for (meshindex_t y=0; y< static_cast<meshindex_t>(_meshsize_pd); y++) {
                data_out[x*_meshsize_pd+y] = 0;
                for (uint_fast8_t j=0; j<_ip; j++) {
                    hi h = _hinfo[y*_ip+j];
                    // the min makes sure not to have out of bounds accesses
                    // casting is to be sure about overflow behaviour
                    const meshindex_t xs = std::min(
                         static_cast<meshindex_t>(_meshsize_pd-1),
                         static_cast<meshindex_t>(static_cast<int32_t>(x+h.index)
                                                - static_cast<int32_t>(_meshsize_pd/2)));
                    data_out[x*_meshsize_kd+y] += data_in[xs*_meshsize_pd+y]
                          * static_cast<meshdata_t>(h.weight);
                }
            }
        }
    } else {
        for (meshindex_t x=0; x< static_cast<meshindex_t>(_meshsize_pd); x++) {
            const meshindex_t offs = x*_meshsize_kd;
            for (meshindex_t y=0; y< static_cast<meshindex_t>(_meshsize_kd); y++) {
                data_out[offs+y] = 0;
                for (uint_fast8_t j=0; j<_ip; j++) {
                    hi h = _hinfo[x*_ip+j];
                    // the min makes sure not to have out of bounds accesses
                    // casting is to be sure about overflow behaviour
                    const meshindex_t ys = std::min(
                        static_cast<meshindex_t>(_meshsize_kd-1),
                        static_cast<meshindex_t>(static_cast<int32_t>(y+h.index)
                                               - static_cast<int32_t>(_meshsize_kd/2)));
                    data_out[offs+y] += data_in[offs+ys]*static_cast<meshdata_t>(h.weight);
                }
            }
        }
    }
    }
}

#ifdef INOVESA_USE_CL
void vfps::KickMap::syncCLMem(clCopyDirection dir)
{
    switch (dir) {
    case clCopyDirection::cpu2dev:
        OCLH::queue.enqueueWriteBuffer(_force_buf,CL_TRUE,0,
                                      sizeof(meshaxis_t)*_meshsize_pd,
                                      _offset.data());
        break;
    case clCopyDirection::dev2cpu:
        OCLH::queue.enqueueReadBuffer(_force_buf,CL_TRUE,0,
                                      sizeof(meshaxis_t)*_meshsize_pd,
                                      _offset.data());
        break;
    }
}
#endif // INOVESA_USE_CL

void vfps::KickMap::updateHM()
{
    #ifdef INOVESA_USE_CL
    if (!OCLH::active)
    #endif
    {
    // gridpoint matrix used for interpolation
    hi* ph = new hi[_it];

    // arrays of interpolation coefficients
    interpol_t* hmc = new interpol_t[_it];

    // translate offset into HM
    for (meshindex_t i=0; i< static_cast<meshindex_t>(_meshsize_pd); i++) {
        meshaxis_t poffs = _meshsize_kd/2+_offset[i];
        meshaxis_t qp_int;
        //Scaled arguments of interpolation functions:
        meshindex_t jd; //numper of lower mesh point from p'
        interpol_t xip; //distance of p' from lower mesh point
        xip = std::modf(poffs, &qp_int);
        jd = qp_int;

        if (jd < static_cast<meshindex_t>(_meshsize_kd)) {
            // create vectors containing interpolation coefficiants
            calcCoefficiants(hmc,xip,_it);

            // renormlize to minimize rounding errors
            // renormalize(hmc.size(),hmc.data());

            // write heritage map
            for (unsigned int j1=0; j1<_it; j1++) {
                unsigned int j0 = jd+j1-(_it-1)/2;
                if(j0 < static_cast<meshindex_t>(_meshsize_kd)) {
                    ph[j1].index = j0;
                    ph[j1].weight = hmc[j1];
                } else {
                    ph[j1].index = _meshsize_kd/2;
                    ph[j1].weight = 0;
                }
                _hinfo[i*_ip+j1] = ph[j1];
            }
        } else {
            for (unsigned int j1=0; j1<_it; j1++) {
                ph[j1].index = _meshsize_kd/2;
                ph[j1].weight = 0;
                _hinfo[i*_ip+j1] = ph[j1];
            }
        }
    }

    delete [] ph;
    delete [] hmc;
    }
}