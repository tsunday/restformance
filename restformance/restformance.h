/*
    FILE: restformance.h
    AUTHOR: Tomasz Niedziela-Brach
*/
#pragma once
#ifdef RESTFORMANCE_EXPORTS
#define RESTFORMANCEDLL_API __declspec(dllexport)
#else
#define RESTFORMANCEDLL_API __declspec(dllimport)
#endif