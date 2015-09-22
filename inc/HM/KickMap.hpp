/******************************************************************************
 * Inovesa - Inovesa Numerical Optimized Vlesov-Equation Solver Application   *
 * Copyright (c) 2014-2015: Patrik Schönfeldt                                 *
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

#ifndef KICKMAP_HPP
#define KICKMAP_HPP

#include "HeritageMap.hpp"

namespace vfps
{

/**
 * @brief The KickMap class allows to apply position dependent forces
 *
 * @todo change to use 1D HeritageMap
 */
class KickMap : public HeritageMap
{
public:
	KickMap(PhaseSpace* in, PhaseSpace* out,
			const unsigned int xsize, const unsigned int ysize,
			const InterpolationType it);

	~KickMap();

public:
	void apply();

	void laser(meshaxis_t amplitude, meshaxis_t pulselen, meshaxis_t wavelen);

protected:
	/**
	 * @brief _force
	 */
	meshaxis_t* _force;

	const meshindex_t _meshysize;
};

}

#endif // KICKMAP_HPP
