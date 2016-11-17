// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#if defined(WIN32)
#include <SDKDDKVer.h>
#endif

#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#define VC_EXTRALEAN
#define GLFW_INCLUDE_GLU
#define GLFW_DLL 1
#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL

#if !(defined(DEBUG) | defined(_DEBUG))
#define NDEBUG
#endif

// TODO: reference additional headers your program requires here
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <algorithm>
#include <array>
//#include <assimp/Importer.hpp>
//#include <assimp/postprocess.h>
//#include <assimp/scene.h>
#include <cassert>
#include <chrono>
#include <fstream>
#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>
#include <initializer_list>
#include <iostream>
#include <map>
#include <memory>
#include <random>
#include <sstream>
#include <string>
#include <vector>