template <typename T>
void f1(T &x)
{
  x += 5;
}

template <typename Iterator, typename F>
void foreach(Iterator ItBegin, Iterator ItEnd, F ope)
{
  auto iter = ItBegin;
  for (; iter != ItEnd; ++iter)
    ope(*iter);
}

template <typename Iterator, typename F, typename... Args>
void foreach(Iterator ItBegin, Iterator ItEnd, F ope, Args&&... args)
{
  auto iter = ItBegin;
  for (; iter != ItEnd; ++iter)
    ope(*iter, std::forward<Args>(args)...);
}