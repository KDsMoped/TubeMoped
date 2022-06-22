/*
  ==============================================================================

    Created: 14 Aug 2019
    Author:  Dominik Köhler

  ==============================================================================
*/

#pragma once

#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>

namespace kdsp {

    const float pi = static_cast<float>(M_PI);

    enum filterType 
    {
        ZoelzerHighpass,
        ZoelzerLowpass,
        ZoelzerBandpass,
        ZoelzerAllpass
    };

    template <typename T>
    class BiquadFilterDF2 
    {
    public:
        BiquadFilterDF2(int order) :
            order(order) 
        {
            for (int l = 0; l < order / 2; ++l) {
                w1.push_back(0);
                w2.push_back(0);
            }
        }

        ~BiquadFilterDF2() {}

        /*
        */
        T process(T inputValue) 
        {
            T x = inputValue;
            T w = 0;
            T y = 0;

            for (int l = 0; l < order / 2; ++l) {
                // Calculate difference equation
                w = x - (a1 * w1[l]) - (a2 * w2[l]);
                y = (b0 * w) + (b1 * w1[l]) + (b2 * w2[l]);

                // Shift buffer values
                x = y;
                w2[l] = w1[l];
                w1[l] = w;
            }

            return y;
        }

        /*
        */
        void calcFilterCoeffs(filterType type, float sr, float f, float q) 
        {
            float k;

            switch (type) {
            case ZoelzerHighpass:
                k = tanf((pi * f) / sr);
                if (order == 1) {
                    b0 = 1.f / (k + 1.f);
                    b1 = b0 * -1.f;
                    b2 = 0.f;
                    a1 = (k - 1.f) / (k + 1.f);
                    a2 = 0.f;
                }
                else {
                    b0 = q / ((pow(k, 2.f) * q) + k + q);
                    b1 = (-2.f * q) / ((pow(k, 2.f) * q) + k + q);
                    b2 = b0;
                    a1 = (2.f * q * (pow(k, 2.f) - 1)) / ((pow(k, 2.f) * q) + k + q);
                    a2 = ((pow(k, 2.f) * q) - k + q) / ((pow(k, 2.f) * q) + k + q);
                }
                break;
            case ZoelzerLowpass:
                k = tanf((pi * f) / sr);
                if (order == 1) {
                    b0 = k / (k + 1.f);
                    b1 = b0;
                    b2 = 0.f;
                    a1 = (k - 1.f) / (k + 1.f);
                    a2 = 0.f;
                }
                else {
                    b0 = (pow(k, 2.f) * q) / ((pow(k, 2.f) * q) + k + q);
                    b1 = (2.f * (pow(k, 2.f) * q)) / ((pow(k, 2.f) * q) + k + q);
                    b2 = b0;
                    a1 = (2.f * q * (pow(k, 2.f) - 1.f)) / ((pow(k, 2.f) * q) + k + q);
                    a2 = ((pow(k, 2.f) * q) - k + q) / ((pow(k, 2.f) * q) + k + q);
                }
                break;
            case ZoelzerBandpass:
                k = tanf((pi * f) / sr);
                b0 = k / ((pow(k, 2.f) * q) + k + q);
                b1 = 0.f;
                b2 = b0 * -1.f;
                a1 = (2.f * q * (pow(k, 2.f) - 1.f)) / ((pow(k, 2.f) * q) + k + q);
                a2 = ((pow(k, 2.f) * q) - k + q) / ((pow(k, 2.f) * q) + k + q);
                break;
            case ZoelzerAllpass:
                k = tanf((pi * f) / sr);
                if (order == 1) {
                    b0 = (k - 1.f) / (k + 1.f);
                    b1 = 1.f;
                    b2 = 0.f;
                    a1 = b0;
                    a2 = 0.f;
                }
                else {
                    b0 = ((pow(k, 2.f) * q) - k + q) / ((pow(k, 2.f) * q) + k + q);
                    b1 = (2.f * q * (pow(k, 2.f) - 1.f)) / ((pow(k, 2.f) * q) + k + q);
                    b2 = 1.f;
                    a1 = b1;
                    a2 = b0;
                }
                break;
            default:
                break;
            }
        }

        /*
        Set filter coefficients that have been calculated externally
        */
        void setFilterCoeffs(T newb0, T newb1, T newb2, T newa1, T newa2) 
        {
            b0 = newb0;
            b1 = newb1;
            b2 = newb2;
            a1 = newa1;
            a2 = newa2;
        }

        /*
        */
        void changeOrder(int newOrder) 
        {
            order = newOrder;
            w1.resize(order / 2);
            w2.resize(order / 2);
        }

        /*
        */
        void clearBuffers() 
        {
            w1.clear();
            w1.resize(order / 2);
            w2.clear();
            w2.resize(order / 2);
        }

    private:
        int order;

        // Biquad Coefficients
        T b0;
        T b1;
        T b2;
        T a1;
        T a2;

        // Buffers
        std::vector<T> w1;
        std::vector<T> w2;
    };
}
