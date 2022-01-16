#pragma once

#include <type_traits>
#include <functional>
#include <tuple>

namespace lio
{
    template <typename Tuple, typename Ret, typename... Args>
    struct Pipe;

    namespace
    {
        template <typename Func>
        struct Function : Function<decltype(&Func::operator())> {};

        template <typename Ret, typename FuncObj, typename... Args>
        struct Function<Ret(FuncObj::*)(Args...) const>
        {
            using Type = std::function<Ret(Args...)>;
            using ReturnType = Ret;
            using ArgsTypes = std::tuple<Args...>;
        };

        template <typename Tuple, typename Ret, typename>
        struct PipeFromTuple {};

        template <typename Tuple, typename Ret, typename... Args>
        struct PipeFromTuple<Tuple, Ret, std::tuple<Args...>>
        {
            using Type = Pipe<Tuple, Ret, Args...>;
        };
    }

    template <typename Tuple, typename Ret, typename... Args>
    struct Pipe
    {
        std::function<Ret(Args...)> func;
        Tuple args;

        template <typename Func>
        auto operator>>(Func new_func)
        {
            return typename PipeFromTuple<std::tuple<decltype(std::apply(func, args))>, typename Function<Func>::ReturnType, typename Function<Func>::ArgsTypes>::Type
            (
                new_func, 
                std::forward_as_tuple(std::apply(func, args))
            );
        }

        template <typename NewRet, typename... NewArgs>
        auto operator>>(NewRet (*new_func)(NewArgs...))
        {
            return Pipe<std::tuple<decltype(std::apply(func, args))>, NewRet, NewArgs...>
            (
                std::function<NewRet(NewArgs...)>(new_func), 
                std::forward_as_tuple(std::apply(func, args))
            );
        }

        template <typename NextArg>
        constexpr auto operator<<(NextArg&& arg)
        {
            return Pipe<decltype(std::tuple_cat(args, std::forward_as_tuple(arg))), Ret, Args...>(func, std::tuple_cat(args, std::forward_as_tuple(arg)));
        }

        auto operator|(Ret& out)
        {
            out = std::apply(func, args);
        }
    };

    template <typename T>
    auto MakePipe(T&& value)
    {
        return Pipe<std::tuple<>, T>([&value]() -> T { return value; });
    }
}