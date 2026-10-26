#pragma once

namespace reference {
    struct sn {
        double ε;
        double θ;
        double A;
        double δx;
        double δy;
        double Φ;
        double m;

        struct {
            double pʼ;
            double qʼ;
            double rʼ;
            double sʼ;
            double tʼ;
            double uʼ;
            double φ;
        } ζ;

        sn(double ε, double θ, double A, double δx, double δy, double Φ, double m);

        double υ(double α);
    };

} // namespace reference

namespace float32 {
    struct sn {
        float ε;
        float θ;
        float A;
        float δx;
        float δy;
        float Φ;
        float m;

        struct {
            float pʼ;
            float qʼ;
            float rʼ;
            float sʼ;
            float tʼ;
            float uʼ;
            float φ;
        } ζ;

        sn(float ε, float θ, float A, float δx, float δy, float Φ, float m);

        float υ(float α);
    };

} // namespace float32

namespace sincos {
    struct sn {
        float ε;
        float θ;
        float A;
        float δx;
        float δy;
        float Φ;
        float m;

        struct {
            float pʼ;
            float qʼ;
            float rʼ;
            float sʼ;
            float tʼ;
            float uʼ;
            float φ;
        } ζ;

        sn(float ε, float θ, float A, float δx, float δy, float Φ, float m);

        void υ(float α, float δα, float out[4]);
    };

} // namespace sincos

namespace sincos2 {
    struct sn {
        float ε;
        float θ;
        float A;
        float δx;
        float δy;
        float Φ;
        float m;

        struct {
            float pʼ;
            float qʼ;
            float rʼ;
            float sʼ;
            float tʼ;
            float uʼ;
            float φ;
        } ζ;

        sn(float ε, float θ, float A, float δx, float δy, float Φ, float m);

        void υ(float α, float δα, float out[4]);
    };

} // namespace sincos2

namespace float4A {
    struct sn {
        float ε;
        float θ;
        float A;
        float δx;
        float δy;
        float Φ;
        float m;

        struct {
            float pʼ;
            float qʼ;
            float rʼ;
            float sʼ;
            float tʼ;
            float uʼ;
            float φ;
        } ζ;

        sn(float ε, float θ, float A, float δx, float δy, float Φ, float m);

        void υ(float α, float δα, float out[4]);
    };

} // namespace float4A

namespace float4XX {
    struct sn {
        double ε;
        double θ;
        double A;
        double δx;
        double δy;
        double Φ;
        double m;

        struct {
            double pʼ;
            double qʼ;
            double rʼ;
            double sʼ;
            double tʼ;
            double uʼ;
            double φ;
        } ζ;

        sn(double ε, double θ, double A, double δx, double δy, double Φ, double m);

        void υ(double α, double δα, float out[4]);
    };

} // namespace float4XX

constexpr double radians(double v) {
    return v * M_PI / 180.0;
}
