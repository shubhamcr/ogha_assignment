#include <exception>
#include <iostream>

// g++ question3.cpp -g -std=c++17 -Wall -Wextra -pthread

// Implemented my own Vector as we are not allowed to use C++ STL.
// This vector will store instances of Version struct
// Please note that this is not a general purpose vector and is
// only supposed to store trivially constructible types.
template <class T>
class Vector {
public:
    Vector() = default;

    ~Vector() {
        free(m_arr);
    }
    
    template <typename... Args>
    void emplace_back(Args&&... args) {
        if (not resizeIfRequired()) {
            return;
        }
        ::new (m_arr + m_size) T(std::forward<Args>(args)...);
        ++m_size;
    }

    const T& operator[](std::size_t i) const {
        return *(m_arr + i);
    }

    T& operator[](std::size_t i) {
        return *(m_arr + i);
    }

    const T& front() const {
        return operator[](0);
    }
    
    std::size_t size() const noexcept {
        return m_size;
    }

private:
    bool resizeIfRequired() {
        if (m_size + 1 <= m_capacity) {
            return true;
        }
        std::size_t newCapacity = (m_capacity + 1) * 2;
        auto newArr = static_cast<T*>(malloc(newCapacity * sizeof(T)));
        if (newArr == nullptr) {
            return false;
        }

        for (std::size_t i = 0; i < m_size; ++i) {
            ::new (newArr + i) T(operator[](i));
        }
        std::swap(m_arr, newArr);
        m_capacity = newCapacity;
        free(newArr);
        return true;
    }
 
    std::size_t m_capacity{0};
    std::size_t m_size{};
    T* m_arr{nullptr};
};

template <typename T>
struct Node {
    T m_data;
    Node* m_next{nullptr};

    Node(const T& data, Node* next = nullptr) : m_data{data}, m_next{next} {}
};

template <typename T>
struct Version {
    Node<T>* m_head{nullptr};
    Node<T>* m_tail{nullptr};

    Version(Node<T>* head, Node<T>* tail) : m_head{head}, m_tail{tail} {}
};

template <typename T>
class Queue {
public:
    ~Queue() {
        if (isEmpty()) {
            return;
        }

        auto curr = m_versionsData.front().m_head;
        while (curr != nullptr) {
            auto next = curr->m_next;
            delete curr;
            curr = next;
        }
    }

    void enqueue(const T& element) {
        if (isEmpty()) {
            m_head = new Node{element};
            m_tail = m_head;
        } else {
            auto node = new Node{element};
            m_tail->m_next = node;
            m_tail = node;
        }
        m_versionsData.emplace_back(m_head, m_tail);
    }

    T dequeue() {
        if (isEmpty()) {
            throw std::runtime_error{"Trying to dequeue element from empty queue!"};
        }
        auto element = m_head->m_data;
        m_head = m_head->m_next;
        m_versionsData.emplace_back(m_head, m_tail);
        return element;
    }

    void print(std::size_t version) {
        if (version == 0 or version > m_versionsData.size()) {
            std::cerr << "Version " << version << " is outside Queue version range." << std::endl;
            return;
        }

        std::cout << "Printing Queue version " << version << std::endl;

        auto head = m_versionsData[version - 1].m_head;
        auto tail = m_versionsData[version - 1].m_tail;
        for (auto curr = head; curr != tail->m_next; curr = curr->m_next) {
            std::cout << curr->m_data << std::endl;
        }
    }

    bool isEmpty() const noexcept { return m_head == nullptr; }

private:
    Node<T>* m_head{nullptr};
    Node<T>* m_tail{nullptr};
    Vector<Version<T>> m_versionsData;
};

int main() {
    Queue<int> q;
    q.enqueue(1);
    q.enqueue(2);
    q.enqueue(3);
    q.dequeue();
    q.dequeue();
    q.dequeue();
    q.enqueue(7);


    for (int version = 0; version <= 8; ++version) {
        q.print(version);
        std::cout << std::endl;
    }
}