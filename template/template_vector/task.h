#include <iostream>

template<typename T>
class Array {
public:
    Array(std::ostream& ostream) 
        : Ostream_(ostream)
        , size_(0)
        , capacity_(2)
        , data_(new T[capacity_])
    {
        //Ostream_ << "Constructed. " << *(this) << "\n";
    }

    Array(const Array<T>& array) 
        : Ostream_(array.Ostream_)
        , size_(array.size_)
        , capacity_(array.capacity_)
        , data_(new T[capacity_])
    {
        CopyData(array.data_, data_);
        //Ostream_ << "Constructed from another Array. " << *(this) << "\n";
    }

    Array(size_t size, std::ostream& ostream = std::cout, T defaultValue = T()) 
        : Ostream_(ostream)
        , size_(size)
        , capacity_(size_ * 2)
        , data_(new T[capacity_])
    {
        for(size_t i = 0; i < size_; ++i){
            data_[i] = defaultValue;
        }
        //Ostream_ << "Constructed with default. " << *(this) << "\n";
    }

    ~Array() {
       // Ostream_ << "Destructed " << size_ << "\n";
        delete[] data_;
    }

    size_t Size() const {
        return size_;
    }

    size_t Capacity() const {
        return capacity_;
    }

    void Reserve(size_t newCapacity) {
        if(newCapacity > capacity_){
            capacity_ = newCapacity;
            T* const newData = new T[newCapacity];
            CopyData(data_, newData);
            delete[] data_;
            data_ = newData;
        }
    }

    void Resize(size_t newSize) {
        if(newSize > capacity_){
            capacity_ = std::max(capacity_ * 2, newSize);
            T* const newData = new T[capacity_];
            CopyData(data_, newData);
            delete[] data_;
            data_ = newData;
        }
        size_ = newSize;
    }

    void PushBack(T value = 0) {
        if(size_ == capacity_){
            capacity_ = capacity_ != 0 ? capacity_ * 2 : 1;
            T* const newData = new T[capacity_];
            CopyData(data_, newData);
            delete[] data_;
            data_ = newData;
        }
        data_[size_] = value;
        ++size_;
    }

    void PopBack() {
        if(size_ > 0){
            --size_;
        }
    }

    void Clear(){
        size_ = 0;
        delete[] data_;
    }

    const T& operator [](const size_t i) const {
        return const_cast<Array*>(this)->operator[](i);
    }

    T& operator [](const size_t i) {
        return data_[i];
    }

    explicit operator bool() const {
        if(size_ == 0){
            return false;
        }
        return true;
    }

    bool operator <(const Array<T>& it) const {
        if(size_ != it.Size()){
            return size_ < it.Size();
        }
        for(size_t i = 0; i < size_; ++i){
            if(data_[i] != it[i]){
                return data_[i] < it[i];
            }
        }
        return false;
    }

    bool operator >(const Array<T>& it) const {
        return it < *this;
    }

    bool operator !=(const Array<T>& it) const {
        return (*this < it) || (it < *this);
    }

    bool operator ==(const Array<T>& it) const {
        return !((*this < it) || (it < *this));
    }

    bool operator <=(const Array<T>& it) const {
        return !(it < *this);
    }

    bool operator >=(const Array<T>& it) const {
        return !(*this < it);
    }

    Array<T>& operator <<(const T& value) {
        PushBack(value);
        return *this;
    }

    Array<T>& operator <<(const Array<T>& it) {
        if(capacity_ < size_ + it.Size()){
            capacity_ = std::max(capacity_, size_ + it.Size());
            T* newData = new T[capacity_];
            CopyData(data_, newData);
            delete[] data_;
            data_ = newData;
        }
        for(size_t i = 0; i < it.Size(); ++i){
            data_[i + size_] = it[i];
        }
        size_ += it.Size();
    }

    bool Insert(size_t pos, const T& value) {
        if(0 <= pos && pos <= size_){
            if(size_ == capacity_){
                capacity_ = size_ != 0 ? capacity_ * 2 : 1;
                T* newData = new T[capacity_];

                for(size_t i = 0; i < pos; ++i){
                    newData[i] = data_[i];
                }
                newData[pos] = value;
                for(size_t i = pos; i < size_; ++i){
                    newData[i + 1] = data_[i];
                }

                delete[] data_;
                data_ = newData;
            }

            for(size_t i = size_; i > pos; --i){
                data_[i] = data_[i - 1];
            }
            data_[pos] = value;
            ++size_;
            return true;
        }
        return false;
    }

    bool Erase(size_t pos) {
        if(0 <= pos && pos < size_){
            for(size_t i = pos; i < size_ - 1; ++i){
                data_[i] = data_[i + 1];
            }
            --size_;
            return true;
        }
        return false;
    }

private:
    std::ostream& Ostream_;
    size_t size_, capacity_;
    T* data_;

    void CopyData(T* source, T* destination){
        for(size_t i = 0; i < size_; ++i){
            destination[i] = source[i];
        }
    }
};

template<typename T>
std::ostream& operator <<(std::ostream& ostream, const Array<T>& array) {
    size_t size = array.Size();
    ostream << "Result Array's capacity is " << array.Capacity() << " and size is " << size;
    if(size != 0){
        ostream << ", elements are: ";
        for(size_t i = 0; i < size; ++i){
            ostream << array[i];
            if(i != size - 1){
                ostream << ", ";
            }
        }
    }
    return ostream;
}

