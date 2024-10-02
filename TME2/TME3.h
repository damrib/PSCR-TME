namespace Iterator_utilities_TME {
    template<typename iterator>
    size_t count(iterator begin, iterator end) {
        size_t cpt;
        for (cpt = 0; begin != end; ++begin, ++cpt) {
            //No Code
        }
        return cpt;
    }  

    template<typename iterator, typename T>
    size_t count_if_equal(iterator begin, iterator end, const T& val) {
        size_t cpt;
        for (cpt = 0; begin != end; ++begin) {
            if ( *begin == val ) ++cpt;
        }
        return cpt;
    }  
}