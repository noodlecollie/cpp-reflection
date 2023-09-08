#include <iostream>
#include <sstream>
#include <tuple>
#include <unordered_map>
#include <utility>

std::unordered_map<const void*, std::string> FunctionSigs;

// Functions for converting a type to a string
template<typename T>
static std::string TypeToString()
{
	return "unknown";
}

template<>
std::string TypeToString<int>()
{
	return "int";
}

template<>
std::string TypeToString<void>()
{
	return "void";
}

template<>
std::string TypeToString<float>()
{
	return "float";
}

template<>
std::string TypeToString<std::string>()
{
	return "std::string";
}

template<>
std::string TypeToString<const std::string*>()
{
	return "const std::string*";
}

template<typename T>
static std::string SanitiseTypeToString()
{
	using NoRefType = typename std::remove_reference<T>::type;
	using NoCVType = typename std::remove_cv<NoRefType>::type;
	return TypeToString<NoCVType>();
}

template<size_t N, typename... ARGS>
std::string TupleTypeToString()
{
	using ElementType = decltype(std::get<N>(std::tuple<ARGS...>()));
	return SanitiseTypeToString<ElementType>();
}

// Functions for splitting tuples
// https://stackoverflow.com/questions/61717459/split-an-stdtuple-on-an-arbitrary-index
template<std::size_t N, std::size_t... Ns, typename... Ts>
auto TailImpl(std::index_sequence<Ns...>, std::tuple<Ts...> t)
{
	return std::make_tuple(std::get<Ns + N>(t)...);
}

template<std::size_t N, typename... Ts>
auto Tail(std::tuple<Ts...> t)
{
	return TailImpl<N>(std::make_index_sequence<sizeof...(Ts) - N>(), t);
}

template<typename T>
static std::string TupleTypesToString(std::tuple<T> t)
{
	return TupleTypeToString<0, T>();
}

template<typename... ARGS>
static std::string TupleTypesToString(std::tuple<ARGS...> t)
{
	using TupleOfOtherTypes = decltype(Tail<1>(std::tuple<ARGS...>()));

	return TupleTypeToString<0, ARGS...>() + ", " + TupleTypesToString(TupleOfOtherTypes());
}

template<typename RET, typename... ARGS>
static std::string FunctionArgTypesToString(RET (*func)(ARGS...))
{
	return TupleTypesToString<ARGS...>(std::tuple<ARGS...>());
}

template<typename RET, typename... ARGS>
static std::string FunctionReturnTypeToString(RET (*func)(ARGS...))
{
	return SanitiseTypeToString<RET>();
}

template<typename RET, typename... ARGS>
static std::string FunctionSignature(RET (*func)(ARGS...), const std::string funcName)
{
	return FunctionReturnTypeToString(func) + " " + funcName + "(" + FunctionArgTypesToString(func) + ")";
}

#define FUNCTION_SIGNATURE(func) FunctionSignature(func, #func)

static void MyFunc(int, int)
{
}

static std::string StringPtrFunc(const std::string*)
{
	return "";
}

int main(int, char**)
{
	FunctionSigs.insert({reinterpret_cast<const void*>(MyFunc), FUNCTION_SIGNATURE(MyFunc)});
	FunctionSigs.insert({reinterpret_cast<const void*>(StringPtrFunc), FUNCTION_SIGNATURE(StringPtrFunc)});

	for ( auto it : FunctionSigs )
	{
		std::cout << "Function '" << it.second << " is at address " << it.first << std::endl;
	}
}
