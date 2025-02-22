# TODO
## Primary :
- Comparsion operators
- Develop branch fix failing CI 
- Iterators
- Hashmap or map ?
- Constructors with new templates
- Documentation

`// Проверяем, является ли тип элемента контейнера std::string` <br>
`template <typename T>` <br>
`using is_string_container = std::is_same<typename T::value_type, std::string>;` <br>
<br>
`// Шаблонный класс` <br>
`template <typename T>` <br>
`class MyClass {` <br>
`public:` <br>
    `// Конструктор принимает только контейнеры со строками` <br>
    `template <typename U, typename = std::enable_if_t<is_string_container<U>::value>>` <br>
    `explicit MyClass(const U& container) {` <br>
        `std::cout << "Создан объект с контейнером строк!\n";` <br>
        `for (const auto& item : container) {` <br>
            `std::cout << item << " ";` <br>
        `}` <br>
        `std::cout << std::endl;` <br>
    `}` <br>
`};` <br>

## Secondary :
- Supported chars (?)
- Benchmarking
