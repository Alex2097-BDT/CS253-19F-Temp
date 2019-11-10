//
// Created by alex on 10/11/19.
//

#include <iostream>
#include <sstream>
#include <string>
#include <tuple>

template <typename... FNs>
class fn_chain {
private:
    const std::tuple<FNs...> functions_;
    const static size_t TUPLE_SIZE = sizeof...(FNs);

    template<typename Arg, std::size_t I>
    auto call_impl(Arg&& arg, const std::index_sequence<I>&) const ->decltype(std::get<I>(functions_)(std::forward<Arg>(arg)))
    {
        return std::get<I>(functions_)(std::forward<Arg>(arg));
    }

    template<typename Arg, std::size_t I, std::size_t... Is>
    auto call_impl(Arg&& arg, const std::index_sequence<I, Is...>&) const ->decltype(call_impl(std::get<I>(functions_)(std::forward<Arg>(arg)), std::index_sequence<Is...>{}))
    {
        return call_impl(std::get<I>(functions_)(std::forward<Arg>(arg)), std::index_sequence<Is...>{});
    }

    template<typename Arg>
    auto call(Arg&& arg) const-> decltype(call_impl(std::forward<Arg>(arg), std::make_index_sequence<sizeof...(FNs)>{}))
    {
        return call_impl(std::forward<Arg>(arg), std::make_index_sequence<sizeof...(FNs)>{});
    }

public:
    fn_chain() : functions_(std::tuple<>()) {}
    fn_chain(std::tuple<FNs...> functions) : functions_(functions) {}

    // add function into chain
    template< typename F >
    inline auto add(const F& f) const
    {
        return fn_chain<FNs..., F>(std::tuple_cat(functions_, std::make_tuple(f)));
    }

    // call whole functional chain
    template <typename Arg>
    inline auto operator()(Arg&& arg) const -> decltype(call(std::forward<Arg>(arg)))
    {
        return call(std::forward<Arg>(arg));
    }
};

// pipe function into functional chain via | operator
template<typename... FNs, typename F>
inline auto operator|(fn_chain<FNs...> && chain, F&& f)
{
    return chain.add(std::forward<F>(f));
}

#define tfn_chain fn_chain<>()

//test code
void test_pipe()
{
    auto f1 = [](int x) { return x + 3; };
    auto f2 = [](int x) { return x * 2; };
    auto f3 = [](int x) { return (double)x / 2.0; };
    auto f4 = [](double x) { std::stringstream ss; ss << x; return ss.str(); };
    auto f5 = [](std::string s) { return "Result: " + s; };
    auto compose_fn = tfn_chain|f1|f2|f3|f4|f5; //compose a chain
    std::cout << compose_fn(2);
}



//int main(int argc, char *argv[]){
//    test_pipe();
//    return 0;
//}
