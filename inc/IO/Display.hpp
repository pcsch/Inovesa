/******************************************************************************
 * Inovesa - Inovesa Numerical Optimized Vlasov-Equation Solver Application   *
 * Copyright (c) 2014-2017: Patrik Schönfeldt                                 *
 * Copyright (c) 2014-2017: Karlsruhe Institute of Technology                 *
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

#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include "defines.hpp"

#include <array>
#include <chrono>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <type_traits>

#ifdef INOVESA_USE_GUI

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#if GLFW_VERSION_MAJOR == 3 // GLFW3
#include <GLFW/glfw3.h>
#else // GLFW2
#include <GL/glfw.h>
#endif // GLFW2
#endif // INOVESA_USE_GUI
#include <vector>

#include "PS/PhaseSpace.hpp"
#include "IO/GUI/GUIElement.hpp"

namespace vfps {

class Display;

class DisplayException : public std::exception {
public:
    DisplayException(std::string msg) : _msg(msg){}

    const char* what() const noexcept override;

private:
    std::string _msg;
};

/**
 * @brief make_display factory function for Display
 * @param gui
 * @param ofname name of result file (will be used for according log file)
 * @param glversion
 * @return pointer to fully initialized Display (may be non-graphical)
 *
 * This factory function will also initialize loging.
 * When no (graphical) display is wanted make_display may
 * just initialize the log, print some status information, or do nothing
 * but to return a nullptr.
 */
std::unique_ptr<Display> make_display(bool gui,
                                      std::string ofname, int cldev,
                                      uint_fast8_t glversion=0);

class Display
{
public:
    /**
     * @brief start_time time stamp of the program start
     *
     * start_time will be used in output and log files to display the
     * progress of execution.
     */
    static std::chrono::system_clock::time_point start_time;

public:
    Display() = delete;

    Display(const Display&) = delete;
    Display(Display&&) = delete;

    Display& operator=(const Display&) = delete;
    Display& operator=(Display&&) = delete;

    /**
     * @brief Display initializes OpenGL
     * @param glversion
     */
    Display(uint_fast8_t glversion);

    /**
     * @brief ~Display() terminats OpenGL (if used)
     */
    ~Display() noexcept;

    #ifdef INOVESA_USE_GUI
        void addElement(std::shared_ptr<GUIElement> newitem);
    #endif // INOVESA_USE_GUI

    void draw();

    static void printText(std::string txt, float silentTime=0.0f);

    #ifdef INOVESA_USE_GUI
        void takeElement(std::shared_ptr<GUIElement> item);
    #endif // INOVESA_USE_GUI

    static std::ofstream logfile;

private:
    GLFWwindow* openWindow(uint_fast8_t glversion);

    #ifdef INOVESA_USE_GUI
    #if GLFW_VERSION_MAJOR == 3
    /**
     * @brief _window pointer to window struct
     *
     * must not be deleted, is owned by GLFW runtime
     */
    GLFWwindow* _window;
    #endif

    std::vector<std::shared_ptr<GUIElement>> _item;
    #endif // INOVESA_USE_GUI

    /**
     * @brief _lastmessage
     */
    static std::chrono::system_clock::time_point _lastmessage;
};

} // namespace vfps

#endif // DISPLAY_HPP

