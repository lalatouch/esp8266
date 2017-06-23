//
//  gesture.cpp
//
//
//  Created by Léa Saviot on 19/06/2017.
//
//

#include <stdio.h>
#include <cmath>
#include "gesture.h"

#include <Arduino.h>

using namespace std;

namespace gesture {

// Add a new accelerometer, gyroscope and magnetometer entry to our data
void Gesture::addNewDataPoint(
	float ax, float ay, float az,
	float gx, float gy, float gz,
	float mx, float my, float mz
) {
	float accelerationNorm = ax*ax + ay*ay + az*az;
	float rotationNorm = gx*gx + gy*gy + gz*gz;
	DataPoint newPoint {
		ax, ay, az,
		gx, gy, gz,
		mx, my, mz,
		accelerationNorm, rotationNorm
	};

	// Store the point only if we are already listening to a gesture or it might
	// be the beginning of a gesture
	if (currentState == STATE_LISTENING_GESTURE) {
		dataPoints.push_back(newPoint);
		if (!isInterestingDataPoint(accelerationNorm, rotationNorm)) {
			numberOfUninterestingDataPoints++;
			if (numberOfUninterestingDataPoints > numberOfUninterestingDataPointsThreshold) {
				currentState = STATE_IDLE;
				analyzeCurrentData();
			}
		}
	}
	else {
		if (isInterestingDataPoint(accelerationNorm, rotationNorm)) {
			currentState = STATE_LISTENING_GESTURE;
			dataPoints.push_back(newPoint);
		}
	}
}

/**
 * Determines if the point might be the beginning of a new gesture, depending of
 * the acceleration and gyroscopic norms
 */
const bool Gesture::isInterestingDataPoint(
	const float accelerationNorm, const float rotationNorm
) {
	return accelerationNorm > accelerationNormThreshold ||
	       rotationNorm > rotationNormThreshold;
}

// Analyze current data to determine if it is a gesture
void Gesture::analyzeCurrentData() {
	currentGesture = dataPoints;
	dataPoints.clear();
	recognizeGesture();
}

// Recognize a gesture
const void Gesture::recognizeGesture() {
	// We first detect the beginning of a rotation to give priority to this type
	// of gesture
	if (isRotationGesture()) {
		logD("Starting to stream rotation.");
		streamRotation();
	}
	else {
		// If it is not a rotation gesture, we look for a linear gesture
		switch (recognizeLinearGesture()) {
			case GESTURE_SHAKE:
				// Send a shake command
				logD("Sending shake command.");
				break;

			case GESTURE_RIGHT:
				// If the state is normal, send a fast forward command
				// If the state is fast backwards, send a stop forward
				logD("Sending gesture right command.");
				break;

			case GESTURE_LEFT:
				// If the state is normal, send a fast backward command
				// If the state is fast forward, send a stop forward
				logD("Sending gesture left command.");
				break;

			case GESTURE_RIGHT_LEFT:
				// Send a next song command
				logD("Sending gesture right left command.");
				break;

			case GESTURE_LEFT_RIGHT:
				// Send a previous song command
				logD("Sending gesture left right command.");
				break;

			default:
				logD("No gesture recognized.");
				break;
		}
	}

}

// Returns true if the gesture has a rotation accelaration above the threshold
//TODO: Also verify that the acceleration is low?
const bool Gesture::isRotationGesture() {
	float rotationNormSum = 0.0f;
	for (int i = 0; i < currentGesture.size() / 4; i++) {
		rotationNormSum += currentGesture[i].gNorm;
	}
	return rotationNormSum >
	       rotationNormThresholdPercent * currentGesture.size() / 4 * rotationNormThreshold;
}

const int Gesture::recognizeLinearGesture() {
	// If the acceleration norms sum is high, the gesture is a shake gesture
	if (getCurrentGestureAccelerationNormSum() > shakeThreshold) {
		return GESTURE_SHAKE;
	}
	else {
		// Check if the gesture is along one direction (maybe both ways) only
		vector<DataPoint> normalizedAcceleration = normalize2DVector(currentGesture);
		if (isLineGesture(normalizedAcceleration)) {
			// If it is a gesture in one way only
			bool toTheRight = isLinearGestureToTheRight();
			if (isUnidirectionalGesture(normalizedAcceleration)) {
				if (toTheRight)
					return GESTURE_RIGHT;
				else
					return GESTURE_LEFT;
			}
			else { // or a gesture that is a round trip
				if (toTheRight)
					return GESTURE_RIGHT_LEFT;
				else
					return GESTURE_LEFT_RIGHT;
			}
		}
		else {
			return GESTURE_NONE;
		}
	}
}

// Send rotation data to change the music level
void Gesture::streamRotation() {
}

// Get the sum of the norms of each acceleration datatpoint from the gesture
const float Gesture::getCurrentGestureAccelerationNormSum() {
	float accelerationNormSum = 0.0f;
	for (int i = 0; i < currentGesture.size(); i++) {
		accelerationNormSum += currentGesture[i].aNorm;
	}
	return accelerationNormSum;
}

// Compute the acceleration differences in the xy plane between consecutive
// data points
vector<Gesture::DataPoint> Gesture::computeAccelerationDifferences(
	vector<DataPoint> accelerationPoints
) {
	vector<DataPoint> result;
	DataPoint newPoint;

	for (int i = 0; i < accelerationPoints.size() - 1; i++) {
		newPoint = {
			accelerationPoints[i+1].ax - accelerationPoints[i].ax,
			accelerationPoints[i+1].ay - accelerationPoints[i].ay
		};
		result.push_back(newPoint);
	}
	return result;
}

// Normalize the acceleration differences vector
// Maybe apply a passe bas ?
vector<Gesture::DataPoint> Gesture::normalize2DVector(vector<DataPoint> points) {
	vector<DataPoint> result;
	DataPoint newPoint;

	for (int i = 0; i < points.size(); i++) {
		float squareNorm = (points[i].ax * points[i].ax) +
		                   (points[i].ay * points[i].ay);
		newPoint.ax = points[i].ax / sqrt(squareNorm);
		newPoint.ay = points[i].ay / sqrt(squareNorm);
		result.push_back(newPoint);
	}
	return result;
}

// Check if the acceleration gives aligned points = if the sum of all absolute
// scalar products is close to the number of points
const bool Gesture::isLineGesture(
	vector<DataPoint> normalizedAccelerationPoints
) {
	float sumScalarProducts = 0.0f;
	int pointsSize = normalizedAccelerationPoints.size();

	for (int i = 0; i < pointsSize - 1; i++) {
		sumScalarProducts += abs(
			normalizedAccelerationPoints[i].ax * normalizedAccelerationPoints[i+1].ax +
			normalizedAccelerationPoints[i].ay * normalizedAccelerationPoints[i+1].ay
		);
	}

	return sumScalarProducts > pointsSize - sumScalarProductsThreshold;
}

// Check if the beginning of the linear gesture is towards the right
const bool Gesture::isLinearGestureToTheRight() {
	//TODO
	return true;
}

// Returns true if the gesture is unidirectional, else it is bi-directional =
// check the scalar product of the mean of the first 1/4 of points with the last
// 1/4 points
bool
Gesture::isUnidirectionalGesture(vector<DataPoint> accelerationPoints) {
	float firstX = 0.f, firstY = 0.f, lastX = 0.f, lastY = 0.f;
	int pointsSize = accelerationPoints.size();

	for (int i = 0; i < pointsSize / 4; i++) {
		firstX += accelerationPoints[i].ax;
		firstY += accelerationPoints[i].ay;
		lastX += accelerationPoints[pointsSize-1-i].ax;
		lastY += accelerationPoints[pointsSize-1-i].ay;
	}

	float scalarProduct = firstX * lastX + firstY * lastY;
	return scalarProduct > 0;
}

/* Helper functions */

// Log a string if debug mode is enabled
void Gesture::logD(string s) {
	if (logDebug) Serial.println(s.c_str());
}

// Initialiise a vector with given values
template<class R>
vector<R> Gesture::initializeVector2(R arg1, R arg2) {
	vector<R> result;
	result.push_back(arg1);
	result.push_back(arg2);
	return result;
}

}
