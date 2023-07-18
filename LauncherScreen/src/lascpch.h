#pragma once

#include <string>
#include <iostream>
#include <stdint.h>

#include "glad/glad.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>