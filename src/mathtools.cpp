//
// Created by Trebing, Peter on 2019-08-22.
//

/*
 * Implementation file for core functions in the library.
 * 
 * Part of the sim physics system.
 * 
 * Copyright (c) Icosagon 2003. All Rights Reserved.
 *
 * This software is distributed under licence. Use of this software
 * implies agreement with all terms and conditions of the accompanying
 * software licence.
 */
#include "mathtools.h"

const Vector3 Vector3::GRAVITY = Vector3(0, -9.81, 0);
const Vector3 Vector3::HIGH_GRAVITY = Vector3(0, -19.62, 0);
const Vector3 Vector3::UP = Vector3(0, 1, 0);
const Vector3 Vector3::RIGHT = Vector3(1, 0, 0);
const Vector3 Vector3::OUT = Vector3(0, 0, 1);
const Vector3 Vector3::X = Vector3(0, 1, 0);
const Vector3 Vector3::Y = Vector3(1, 0, 0);
const Vector3 Vector3::Z = Vector3(0, 0, 1);

/*
 * Definition of the sleep epsilon extern.
 */
double sleepEpsilon = ((double) 0.3);

/*
 * Functions to change sleepEpsilon.
 */
void setSleepEpsilon(double value) {
    sleepEpsilon = value;
}

double getSleepEpsilon() {
    return sleepEpsilon;
}


Matrix3 Matrix3::linearInterpolate(const Matrix3 &a, const Matrix3 &b, double prop) {
    Matrix3 result;
    for (unsigned i = 0; i < 9; i++) {
        result.data[i] = a.data[i] * (1 - prop) + b.data[i] * prop;
    }
    return result;
}

