#ifndef _KompasAPI_KompasEvent_h_
#define _KompasAPI_KompasEvent_h_

#include <utility>
#include <atomic>

template <typename Signature>
class KompasEvent;

template <typename Ret, typename... Args>
class KompasEvent<Ret(Args...)> {
private:
    // Базовый класс для стирания типов и подсчета ссылок
    struct WrapperBase {
        std::atomic<int> refcount{1};
        virtual Ret Execute(Args... args) = 0;
        virtual ~WrapperBase() = default;
    };

    // Обертка для одного обработчика
    template <typename F>
    struct Wrapper : WrapperBase {
        F fn;
        Wrapper(F&& func) : fn(std::forward<F>(func)) {}
        Ret Execute(Args... args) override {
            return fn(std::forward<Args>(args)...);
        }
    };

    // Обертка для цепочки обработчиков
    template <typename F>
    struct ChainWrapper : WrapperBase {
        KompasEvent previous;
        F current;

        ChainWrapper(const KompasEvent& prev, F&& curr) 
            : previous(prev), current(std::forward<F>(curr)) {}

        Ret Execute(Args... args) override {
            // Защита от удаления "себя" во время выполнения (как AtExit в U++)
            struct Guard {
                ChainWrapper* ptr;
                ~Guard() {
                    if (ptr->refcount.fetch_sub(1) == 1) {
                        delete ptr;
                    }
                }
            } guard{this};

            WrapperBase::refcount.fetch_add(1);
            
            // Вызываем предыдущую цепочку (результат игнорируется или перезаписывается)
            if (previous) {
                previous(std::forward<Args>(args)...);
            }
            
            // Возвращаем результат последнего обработчика
            return current(std::forward<Args>(args)...);
        }
    };

    WrapperBase* ptr = nullptr;

    void Release() {
        if (ptr && ptr->refcount.fetch_sub(1) == 1) {
            delete ptr;
        }
    }

    void Copy(const KompasEvent& other) {
        ptr = other.ptr;
        if (ptr) {
            ptr->refcount.fetch_add(1);
        }
    }

public:
    KompasEvent() : ptr(nullptr) {}
    ~KompasEvent() { Release(); }

    // Конструктор из лямбды / функтора
    template <typename F>
    KompasEvent(F&& fn) {
        ptr = new Wrapper<typename std::decay<F>::type>(std::forward<F>(fn));
    }

    // Копирование (увеличивает refcount)
    KompasEvent(const KompasEvent& other) { Copy(other); }
    KompasEvent& operator=(const KompasEvent& other) {
        if (this != &other) {
            Release();
            Copy(other);
        }
        return *this;
    }

    // Перемещение (быстрое, без изменения refcount)
    KompasEvent(KompasEvent&& other) noexcept : ptr(other.ptr) { other.ptr = nullptr; }
    KompasEvent& operator=(KompasEvent&& other) noexcept {
        if (this != &other) {
            Release();
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    // Добавление обработчика в цепочку (оператор <<)
    template <typename F>
    KompasEvent& operator<<(F&& fn) {
        WrapperBase* old_ptr = ptr;
        ptr = new ChainWrapper<typename std::decay<F>::type>(*this, std::forward<F>(fn));
        
        // Освобождаем старый указатель (уменьшаем refcount на 1)
        if (old_ptr && old_ptr->refcount.fetch_sub(1) == 1) {
            delete old_ptr;
        }
        return *this;
    }

    // Вызов события
    Ret operator()(Args... args) const {
        if (ptr) {
            return ptr->Execute(std::forward<Args>(args)...);
        }
        return Ret(); // Возврат по умолчанию, если никто не подписан
    }

    explicit operator bool() const { return ptr != nullptr; }
    
    void Clear() {
        Release();
        ptr = nullptr;
    }
};

#endif
