#include <fastread.h>
    
namespace fastread{

    ReadConnectionSource::ReadConnectionSource(SEXP id, char sep, char quote, char esc) : 
        Base(sep, quote, esc), 
        con(getConnection(as<int>(id))), 
        chunk_size(10000), 
        buffer(chunk_size, '\0' ), 
        data(&buffer[0]), n(0)
    {
        // TODO: we need to consume the push back first
        n = con->read(data,1,chunk_size,con);
        find_last_line() ;
    }
    
    bool ReadConnectionSource::ensure_full_line(){
        if( p > last_full_line && more() ) return true ;    
        return p < last_full_line ;
    }
    
    void ReadConnectionSource::find_last_line(){
        Base::set(data, data + n) ;
        last_full_line = data + n - 1 ;
        while( *last_full_line != '\n' ) --last_full_line;
    }
        
    bool ReadConnectionSource::more(){
        int nchars = n-(p-data) ;
        std::memmove(data, p, nchars) ;
        n = con->read(data + nchars, 1, chunk_size - nchars, con);
        bool done = n < chunk_size - nchars;  
        n = n + nchars ;
        
        find_last_line() ;
        return !done ;
    }
    
}
