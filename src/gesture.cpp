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
		(*dataPoints)[dataPointsIndex++] = newPoint;
		if (dataPointsIndex >= MAX_POINTS) {
			dataPointsIndex = 0;
			dataPointsLooped = true;
		}

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
			(*dataPoints)[dataPointsIndex++] = newPoint;
			if (dataPointsIndex >= MAX_POINTS) {
				dataPointsIndex = 0;
				dataPointsLooped = true;
			}
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
	// Swap buffers
	::swap(dataPoints, currentGesture);
	gestureIndex = dataPointsIndex;
	gestureLooped = dataPointsLooped;
	dataPointsIndex = 0;
	dataPointsLooped = false;
	recognizeGesture();
}

// Recognize a gesture
const void Gesture::recognizeGesture() {
	computeNormSum();

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
	return gNormSum >
	       rotationNormThresholdPercent * currentGesture->size() / 4 * rotationNormThreshold &&
	       aNormSum < accelerationNormThreshold;
}

const int Gesture::recognizeLinearGesture() {
	// If the acceleration norms sum is high, the gesture is a shake gesture
	if (aNormSum > shakeThreshold) {
		return GESTURE_SHAKE;
	}
	else {
		// Check if the gesture is along one direction (maybe both ways) only
		normalizeGesture();
		if (isLineGesture()) {
			// If it is a gesture in one way only
			bool toTheRight = isLinearGestureToTheRight();
			if (isUnidirectionalGesture()) {
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

void Gesture::computeNormSum() {
	aNormSum = gNormSum = 0.f;

	for (const auto &p : *currentGesture) {
		aNormSum += p.aNorm;
		gNormSum += p.gNorm;
	}
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

void Gesture::normalizeGesture() {
	for (auto &point : *currentGesture) {
		float norm = sqrt(point.ax * point.ax + point.ay * point.ay);
		point.ax /= norm;
		point.ay /= norm;
	}
}

// Check if the acceleration gives aligned points = if the sum of all absolute
// scalar products is close to the number of points
const bool Gesture::isLineGesture() {
	float sumScalarProducts = 0.0f;
	int pointsSize = currentGesture->size();

	for (int i = 0; i < pointsSize - 1; i++) {
		auto &cur = (*currentGesture)[i],
		     &next = (*currentGesture)[i+1];
		sumScalarProducts += abs(cur.ax * next.ax + cur.ay + next.ay);
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
bool Gesture::isUnidirectionalGesture() {
	float firstX = 0.f, firstY = 0.f, lastX = 0.f, lastY = 0.f;
	int pointsSize = currentGesture->size();

	for (int i = gestureLooped ? gestureIndex - 1 : 0;
	     i < gestureIndex + (gestureLooped ? MAX_POINTS : 0);
	     i++) {
		const auto &first = (*currentGesture)[i % MAX_POINTS],
		           &last = (*currentGesture)[pointsSize - 1 - (i % MAX_POINTS)];

		firstX += first.ax;
		firstY += first.ay;
		lastX += last.ax;
		lastY += last.ay;
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
