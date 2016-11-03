//
// Created by erik on 10/2/16.
//

#ifndef SOI_DIMENSIONS_H_H
#define SOI_DIMENSIONS_H_H

#include <ratio>
#include <iosfwd>

namespace std
{
    template<class O, intmax_t N, intmax_t D>
    basic_ostream<O>& operator<<( basic_ostream<O>& stream, ratio<N, D> ratio );
}

namespace spatacs
{
    namespace physics
    {
        namespace dimensions
        {
            template<class T>
            struct DimBase { };

            template<class LengthDim, class TimeDim, class MassDim>
            struct Dimension_t : public DimBase<Dimension_t<LengthDim, TimeDim, MassDim>>
            {
                using length = LengthDim;
                using time   = TimeDim;
                using mass   = MassDim;
            };

            using Zero = std::ratio<0, 1>;
            using One  = std::ratio<1, 1>;

            // calculation with dimensions
            template<class D1, class D2, template<class, class> class OP>
            struct OperationDim
            {
                using new_len  = OP<typename D1::length, typename D2::length>;
                using new_time = OP<typename D1::time,   typename D2::time>;
                using new_mass = OP<typename D1::mass,   typename D2::mass>;
                using type  = Dimension_t<new_len, new_time, new_mass>;
            };


            template<class A, class B>
            using mul_t = typename OperationDim<A, B, std::ratio_add>::type;

            template<class A, class B>
            using div_t = typename OperationDim<A, B, std::ratio_subtract>::type;

            template<class A, int P, int Q>
            using pow_t = typename OperationDim<A, Dimension_t<std::ratio<P, Q>, std::ratio<P, Q>, std::ratio<P, Q>>, std::ratio_multiply>::type;

            template<class T, class U>
            constexpr bool dimensions_equal()
            {
                return std::ratio_equal<typename T::length, typename U::length>::value &&
                       std::ratio_equal<typename T::time, typename U::time>::value &&
                       std::ratio_equal<typename T::mass, typename U::mass>::value;
            }

            // pre defined useful dimensions
            using dimless_t = Dimension_t<Zero, Zero, Zero>;
            using length_t  = Dimension_t<One,  Zero, Zero>;
            using time_t    = Dimension_t<Zero, One,  Zero>;
            using mass_t    = Dimension_t<Zero, Zero, One>;

            template<class T>
            using rate_dim_t = div_t<T, time_t>;

            template<class T>
            using inverse_dim_t = div_t<dimless_t, T>;

            using velocity_t     = rate_dim_t<length_t>;
            using impulse_t      = mul_t<velocity_t, mass_t>;
            using acceleration_t = rate_dim_t<velocity_t>;
            using force_t        = mul_t<acceleration_t, mass_t>;
            using area_t         = mul_t<length_t, length_t>;
            using energy_t       = mul_t<force_t, length_t>;
            using power_t        = rate_dim_t<energy_t>;

            template<class O>
            struct unit_symbols;

            template<>
            struct unit_symbols<char>
            {
                constexpr static const char* m   = "m";
                constexpr static const char* kg  = "kg";
                constexpr static const char* sec = "s";
                constexpr static const char* exp = "^";
                constexpr static const char* div = "/";
                constexpr static const char* sqr = "²";
            };

            template<>
            struct unit_symbols<wchar_t>
            {
                constexpr static const wchar_t* m   = L"m";
                constexpr static const wchar_t* kg  = L"kg";
                constexpr static const wchar_t* sec = L"s";
                constexpr static const wchar_t* exp = L"^";
                constexpr static const wchar_t* div = L"/";
                constexpr static const wchar_t* sqr = L"²";
            };

            template<class O, class R>
            void output_dimension(std::basic_ostream<O>& stream, const O* symbol, R&& r)
            {
                if(std::ratio_equal<R, Zero>::value) {
                    return;
                }

                stream << symbol;
                if (std::ratio_equal<R, One>::value) {
                    return;
                }
/*
                if (std::ratio_equal<R, std::ratio<2, 1>>::value) {
                    stream << unit_symbols<O>::sqr;
                    return;
                }
*/
                stream << unit_symbols<O>::exp << r;
            }

            template<class O, class T>
            std::basic_ostream<O>& operator<<( std::basic_ostream<O>& stream, DimBase<T> d )
            {
                using len_t  = typename T::length;
                using mass_t = typename T::mass;
                using time_t = typename T::time;

                output_dimension(stream, unit_symbols<O>::kg, mass_t{});
                output_dimension(stream, unit_symbols<O>::m, len_t{});
                output_dimension(stream, unit_symbols<O>::sec, time_t{});
                return stream;
            }
        }
    }
}

template<class O, intmax_t N, intmax_t D>
std::basic_ostream<O>& std::operator<<( std::basic_ostream<O>& stream, std::ratio<N, D> ratio )
{
    using ratio_t = std::ratio<N, D>;
    if( ratio_t::den != 1 ) {
        return stream << ratio_t::num << spatacs::physics::dimensions::unit_symbols<O>::div << ratio_t::den;
    }else
    {
        return stream << ratio_t::num;
    }
};


#endif //SOI_DIMENSIONS_H_H
