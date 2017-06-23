//
//  gesture.h
//
//
//  Created by Léa Saviot on 19/06/2017.
//
//

#ifndef _GESTURE_H
#define _GESTURE_H

#include <array>
#include <vector>
#include <string>

using namespace std;

#define MAX_POINTS 256

namespace gesture {

class Gesture {

public:
	typedef struct DataPoint {
		float ax, ay, az, gx, gy, gz, mx, my, mz;
		float aNorm, gNorm;
	} DataPoint;

	// Add a new acceleration, gyroscope and magnetometer data point
	void addNewDataPoint(
		float ax, float ay, float az,
		float gx, float gy, float gz,
		float mx, float my, float mz
	);
	const void recognizeGesture();

	Gesture()
	 : dataPointsLooped(false), gestureLooped(false),
	   dataPointsIndex(0), gestureIndex(0),
	   dataPoints(&buffer1), currentGesture(&buffer2) {}

private:
	bool logDebug = true;

	/* Thresholds for determining if a gesture is significative */

	// If a gesture has more than this threshold of uninteresting points, we
	// consider the gesture is finished
	static constexpr int numberOfUninterestingDataPointsThreshold = 100;
	// If the rotation norm of a datapoint is superior to this threshold, it is
	// a rotation
	static constexpr float rotationNormThreshold = 2.f * 2.f;
	// If the persentage of rotation data points is superior to this threshold
	// in the first quarter of the gesture, it is a rotation.
	static constexpr float rotationNormThresholdPercent = 0.75f;
	// If the acceleration norm of a datapoint is superior to this threshold,
	// it is a significative gesture
	static constexpr float accelerationNormThreshold = 3.f * 3.f;
	static constexpr float shakeThreshold = 3.f * 3.f;
	static constexpr float sumScalarProductsThreshold = 5.0f;

	bool dataPointsLooped, gestureLooped;
	int dataPointsIndex, gestureIndex;
	array<DataPoint, MAX_POINTS> buffer1, buffer2;
	array<DataPoint, MAX_POINTS> *dataPoints, *currentGesture;
	int numberOfUninterestingDataPoints = 0;
	float aNormSum, gNormSum;

	vector<DataPoint> computeAccelerationDifferences(
		vector<DataPoint> accelerationPoints
	);
	const bool isInterestingDataPoint(
		const float accelerationNorm, const float rotationNorm
	);
	const bool isRotationGesture();
	const int recognizeLinearGesture();
	const bool isLineGesture();
	const bool isLinearGestureToTheRight();
	bool isUnidirectionalGesture();
	void analyzeCurrentData();
	void streamRotation();
	void normalizeGesture();
	void computeNormSum();

	// Gestures enumeration
	enum gestures {
		GESTURE_NONE,
		GESTURE_SHAKE,
		GESTURE_RIGHT,
		GESTURE_LEFT,
		GESTURE_RIGHT_LEFT,
		GESTURE_LEFT_RIGHT
	};
	int previousGesture = GESTURE_NONE;

	// State machine
	enum states {
		STATE_IDLE,
		STATE_LISTENING_GESTURE
	};
	int currentState = STATE_IDLE;

	// Helper function
	void logD(string s); // Log a debug string
	template<class R>
		vector<R> initializeVector2(R arg1, R arg2);

};
}

#endif /* defined(____gesture__) */
