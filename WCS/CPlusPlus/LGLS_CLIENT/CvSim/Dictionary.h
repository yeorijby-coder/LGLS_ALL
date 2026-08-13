
#ifndef __DP__DICTIONARY__
#define __DP__DICTIONARY__

#define __DICT_NULL (0)

template<typename _UNITKEY, typename _UNITVALUE>
class DictionaryUnit{
private:
    _UNITKEY* _keyUnit = __DICT_NULL;
    _UNITVALUE* _valueUnit = __DICT_NULL;
    int _keySize = __DICT_NULL, _valueSize = __DICT_NULL, _keyCapacity = 1, _ValueCapacity = 1;

protected:
    void __addKey(_UNITKEY _keyUnit){
        if(_keySize + 1 >= _keyCapacity){
            _keyCapacity *= 2;
            _UNITKEY* tmp = new _UNITKEY[_keyCapacity];
            for(int i=0;i<_keySize;i++) tmp[i] = this->_keyUnit[i];
            tmp[_keySize++] = _keyUnit;
            delete[] this->_keyUnit;
            this->_keyUnit = tmp;
        }
        else this->_keyUnit[_keySize++] = _keyUnit;
    }
    void __addValue(_UNITVALUE _valueUnit){
        if(_valueSize + 1 >= _ValueCapacity){
            _ValueCapacity *= 2;
            _UNITVALUE* tmp = new _UNITVALUE[_ValueCapacity];
            for(int i=0;i<_valueSize;i++) tmp[i] = this->_valueUnit[i];
            tmp[_valueSize++] = _valueUnit;
            delete[] this->_valueUnit;
            this->_valueUnit = tmp;
        }
        else this->_valueUnit[_valueSize++] = _valueUnit;
    }

public:
    DictionaryUnit() { }
    _UNITKEY GetKey(int index){ return this->_keyUnit[index]; }
    _UNITVALUE GetValue(int index){ return this->_valueUnit[index]; }
    int KeyCount(){ return this->_keySize; }
    int ValueCount(){ return this->_valueSize; }
    int KeyCapacity(){ return this->_keyCapacity; }
    int ValueCapacity(){ return this->_ValueCapacity; }
};

template<typename _KEY, typename _VALUE>
class Dictionary : public DictionaryUnit<_KEY, _VALUE> {
private:
    _KEY* _key = __DICT_NULL;
    _VALUE* _value = __DICT_NULL;
    int _size = __DICT_NULL, _capacity = 1;

public:
    Dictionary() { }
    ~Dictionary() { delete[] this->_key, this->_value; }
    Dictionary(_KEY _key, _VALUE _value) { PushKey(_key, _value); }
    void PushKey(_KEY _key, _VALUE _value) {
        if(_size + 1 >= _capacity){
            _capacity *= 2;

            _KEY* tmpkey = new _KEY[_capacity];
            _VALUE* tmpvalue = new _VALUE[_capacity];

            for(int i=0;i<_size;i++) tmpkey[i] = this->_key[i];
            for(int i=0;i<_size;i++) tmpvalue[i] = this->_value[i];

            tmpkey[_size] = _key;
            tmpvalue[_size] = _value;

            delete[] this->_key, this->_value;

            this->_key = tmpkey;
            this->_value = tmpvalue;

            _size+=1;
        }
        else {
            this->_key[_size] = _key;
            this->_value[_size] = _value;

            _size+=1;
        }
    }
    int PopKey(_KEY _key) {
        if(_size > 0){
            int cnt = 0, chek = 0;
            _KEY* tmpkey = new _KEY[_capacity];
            _VALUE* tmpvalue = new _VALUE[_capacity];

            for(int i=0;i<_size;i++){
                if(this->_key[i] == _key) {
                    chek += 1;
                    continue;
                }
                tmpkey[cnt] = this->_key[i];
                tmpvalue[cnt] = this->_value[i];
                cnt+=1;
            }

            if(chek != 0){
                delete[] this->_key,  this->_value;

                this->_key = tmpkey;
                this->_value = tmpvalue;

                _size-=chek;
                return chek;
            }
            return -1;
        }
    }
    DictionaryUnit<_KEY, _VALUE> GetKeyFromValue(_VALUE _value){
        Dictionary<_KEY, _VALUE>klist;
        for(int i=0;i<_size;i++) if(this->_value[i] == _value) klist.__addKey(this->_key[i]);
        return klist;
    }
    DictionaryUnit<_KEY, _VALUE> GetValueFromKey(_KEY _key){
        Dictionary<_KEY, _VALUE>vlist;
        for(int i=0;i<_size;i++) if(this->_key[i] == _key) vlist.__addValue(this->_value[i]);
        return vlist;
    }
    _KEY GetKey(int index){ return this->_key[index]; }
    _VALUE GetValue(int index){ return this->_value[index]; }
    void SwapKey(int index1, int index2){
        _KEY tmp = this->_key[index1];
        this->_key[index1] = this->_key[index2];
        this->_key[index2] = tmp;
    }
    int Capacity(){ return this->_capacity; }
    int Count(){ return this->_size; }
    bool operator==(Dictionary<_KEY, _VALUE>& dict){
        if(this->_size == dict.Count()){
            for(int i=0;i<dict.Count();i++) if(this->_key[i] != dict.GetKey(i) || this->_value[i] != dict.GetValue(i)) return false;
        }
        else return false;
        return true;
    }
    bool operator!=(Dictionary<_KEY, _VALUE>& dict){
        if(this->_size == dict.Count()){
            for(int i=0;i<dict.Count();i++) if(this->_key[i] != dict.GetKey(i) || this->_value[i] != dict.GetValue(i)) return true;
        }
        else return true;
        return false;
    }
    void Copy(Dictionary<_KEY, _VALUE>& Dist){
        this->_capacity = 1;
        this->_size = 0;
        delete[] this->_key, this->_value;
        for(int i=0;i<Dist.Count();i++) PushKey(Dist.GetKey(i), Dist.GetValue(i));
    }
};

#endif /*__DP__DICTIONARY__*/