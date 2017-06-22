//
//  gesture.h
//
//
//  Created by Léa Saviot on 19/06/2017.
//
//

#ifndef ____gesture__
#define ____gesture__

#include <vector>

using namespace std;

namespace gestureNS {
    class gesture {

        
    public:
        void addNewDataPoint(float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz, float accelerationNorm, float rotationNorm);
        const int recognizeGesture();
        
    private:
        bool logDebug = true;
        
        /* Thresholds for determining if a gesture is significative */
        
        // If a gesture has more than this threshold of uninteresting points, we consider the gesture is finished
        static constexpr int numberOfUninterestingDataPointsThreshold = 100;
        // If the rotation norm of a datapoint is superior to this threshold, it is a rotation
        static constexpr float rotationNormThreshold = 2.0f;
        // If the persentage of rotation data points is superior to this threshold in the first quarter of the gesture, it is a rotation.
        static constexpr float rotationNormThresholdPercent = 0.75f;
        // If the acceleration norm of a datapoint is superior to this threshold, it is a significative gesture
        static constexpr float accelerationNormThreshold = 2.0f;
        static constexpr float shakeThreshold = 2.0f;
        static constexpr float sumScalarProductsThreshold = 5.0f;
        
        vector<vector<float> > dataPoints;
        vector<vector<float> > currentGesture;
        int numberOfUninterestingDataPoints = 0;
        
        void addNewDataPoint(float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz);
        vector<vector<float> > computeAccelerationDifferences(vector<vector<float> > accelerationPoints);
        const bool isInterestingDataPoint(const float accelerationNorm, const float rotationNorm);
        const bool isRotationGesture();
        const int recognizeLinearGesture();
        const bool isLineGesture(vector<vector<float> > normalizedAccelerationPoints);
        const bool isLinearGestureToTheRight();
        bool isUnidirectionalGesture(vector<vector<float> > accelerationPoints);
        void analyzeCurrentData();
        void streamRotation();
        const float getCurrentGestureAccelerationNormSum();
        
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
        
        // Maths functions
        vector<vector<float> > normalize2DVector(vector<vector<float> > points);
        
        // Helper function
        void logD(string s); // Log a debug string
        template<class R>
        vector<R> initializeVector2(R arg1, R arg2);
        
    };
}

#endif /* defined(____gesture__) */
