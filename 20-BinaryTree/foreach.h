template <typename T>
void f1(T& x)
{
  x += 5;
}

template <typename Iterator, typename F>
void foreach(Iterator ItBegin, Iterator ItEnd, F ope)
{
  for (auto iter = ItBegin; iter != ItEnd; ++iter)
    ope(*iter);
}

template <typename Iterator, typename F, typename... Args>
void foreach(Iterator ItBegin, Iterator ItEnd, F ope, Args&&... args)
{
  for (auto iter = ItBegin; iter != ItEnd; ++iter)
    invoke(ope, *iter, std::forward<Args>(args)...);
}