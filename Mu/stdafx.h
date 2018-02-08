// stdafx.h: archivo de inclusión de los archivos de inclusión estándar del sistema
// o archivos de inclusión específicos de un proyecto utilizados frecuentemente,
// pero rara vez modificados
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Excluir material rara vez utilizado de encabezados de Windows
// Archivos de encabezado de Windows:
#include <windows.h>

// Archivos de encabezado en tiempo de ejecución de C
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <math.h>


// TODO: mencionar aquí los encabezados adicionales que el programa necesita
#include "..\engine\Engine.h"
#ifdef _DEBUG
#pragma comment(lib,"..\\Debug\\Engine.lib")
#else
#pragma comment(lib,"..\\Release\\Engine.lib")
#endif