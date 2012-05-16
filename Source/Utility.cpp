#include "../JuceLibraryCode/JuceHeader.h"

/*
#define M_E         2.71828182845904523536028747135266250 
#define M_LOG2E     1.44269504088896340735992468100189214
#define M_LOG10E    0.434294481903251827651128918916605082
#define M_LN2       0.693147180559945309417232121458176568
#define M_LN10      2.30258509299404568401799145468436421
#define M_PI        3.14159265358979323846264338327950288
#define M_PI_2      1.57079632679489661923132169163975144
#define M_PI_4      0.785398163397448309615660845819875721 
#define M_1_PI      0.318309886183790671537767526745028724 
#define M_2_PI      0.636619772367581343075535053490057448
#define M_2_SQRTPI  1.12837916709551257389615890312154517 
#define M_SQRT2     1.41421356237309504880168872420969808
#define M_SQRT1_2   0.707106781186547524400844362104849039
*/ 

// Linear interpolation, used to crossfade the gain table.
static inline float UTIL_LINTERP( float amount, float a, float b ) {
    return (1-amount)*a + amount*b;
}

/*
 * It works making the diference add its abs, so if the diff, is negative it annulates. then it
 * fixes everything from this anulation to return a or b.
 */
static inline float UTIL_MIN( float a, float b) {
    a = b - a;
    a += fabs( a );
    a *= 0.5;
    a = b - a;
    return a;
}

/*
 * Clamp without branching 
 * If input - _limit < 0, then it really substracts, and the 0.5 to make it half the 2 inputs.
 * If > 0 then they just cancel, and keeps input normal.
 * The easiest way to understand it is check what happends on both cases.
 */
static inline float UTIL_SATURATE(float input) {
    float x1 = fabs( input + 0.95 );
    float x2 = fabs( input - 0.95 );
    return 0.5 * (x1 - x2);
}

/*
static void UTIL_ANTIALIASING_FILTER(const real_T b[11], const real_T x[2], real_T y[2])
{
    real_T dbuffer[11];
    
    int32_T k;
    int32_T j;
    
    
    memset(&dbuffer[1], 0, 10U * sizeof(real_T));
    for (j = 0; j < 2; j++) {
        for (k = 0; k < 10; k++) {
            dbuffer[k] = dbuffer[k + 1];
        }
        
        dbuffer[10] = 0.0;
        for (k = 0; k < 11; k++) {
            dbuffer[k] += x[j] * b[k];
        }
        
        y[j] = dbuffer[0];
    }
}
 */ 
