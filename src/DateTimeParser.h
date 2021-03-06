#ifndef FASTREAD_DATETIMEPARSER_H_
#define FASTREAD_DATETIMEPARSER_H_

template <typename Class>
class DateTimeParser{
public:

  /**
   * The number of days since the epoc
   */
  double parse_Date( char* start, char* end_ ){
    p = start ;
    end = end_ ;

    double res = 0 ;

    skip_non_digit() ;
    if( !has_more() ) return res ;

    // year
    int y = read_year() ;
    if( y >= 0 ){
      // feb 29th
      res += y * 365 + y/4 ;
    }

    skip_non_digit() ;

    if( !has_more() ) return res ;

    // month
    int m = read_month() ;
    res += days_at_month_start(m) ;

    // feb 29th from previous years
    if( m > 2 && (y&3) == 2 ){
      res += 1 ;
    }

    skip_non_digit() ;

    // day
    if( !has_more() ) return res ;
    int d = read_day() ;
    if( d > 1 ){
      res += (d - 1);
    }

    return res ;
  }

  double parse_Time( char* start, char* end_ ){
    p = start ;
    end = end_ ;
    double res = 0 ;

    skip_non_digit() ;
    if( !has_more() ) return res ;

    // hour
    res += read_hour() * HOUR ;
    skip_non_digit() ;

    // minutes
    if( !has_more() ) return res ;
    res += read_minute() * MINUTE ;
    skip_non_digit() ;

    // seconds
    if( !has_more() ) return res ;

    res += read_second() ;
    return res ;
  }


  /**
   * The number of seconds since the epoc
   */
  double parse_POSIXct( char* start, char* end_ ){
    p = start ;
    end = end_ ;

    double res = 0.0 ;

    skip_non_digit() ;
    if( !has_more() ) return res ;

    // year
    int y = read_year() ;
    if( y >= 0 ){
      // feb 29th from previous years
      res += y*YEAR + ( (y+1)/4 ) * DAY ;
    }

    skip_non_digit() ;
    if( !has_more() ) return res ;

    // month
    int m = read_month() ;
    res += seconds_at_month_start(m) ;

    // feb 29th
    if( m > 2 && (y&3) == 2 ) res += DAY ;

    skip_non_digit() ;
    if( !has_more() ) return res ;

    // day
    int d = read_day() ;
    if( d > 1){
      res +=  (d-1) * DAY ;
    }
    skip_non_digit() ;

    res += parse_Time(p, end);

    return res ;
  }

private:
  char* p ;
  char* end ;

  inline int seconds_at_month_start(int m){
    static const int data[] = { 0, 0, 2678400, 5097600, 7776000, 10368000, 13046400, 15638400,
                                18316800, 20995200, 23587200, 26265600, 28857600, 31536000 };
    return data[m] ;
  }

  inline int days_at_month_start(int m){
    static const int data[] = { 0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 } ;
    return data[m] ;
  }

  inline bool valid_digit(){
    return *p >= '0' && *p <= '9' ;
  }

  inline int digit_value(){
    return *p - '0' ;
  }

  static const int YEAR = 31536000 ;
  static const int DAY = 86400 ;
  static const int HOUR = 3600 ;
  static const int MINUTE = 60;

  inline int read_year(){
    int y = read_int() ;
    if( y < 100 ) y+=2000 ;
    y -= 1970;
    return y ;
  }

  inline int read_month(){
    return read_int() ;
  }

  inline int read_day(){
    return read_int() ;
  }

  inline int read_hour(){
    return read_int() ;
  }

  inline int read_minute(){
    return read_int() ;
  }

  inline int read_second(){
    return read_int() ;
  }

  inline int read_int(){
    int m = 0 ;
    while( valid_digit() && has_more() ) {
      m = m*10 + digit_value();
      p++ ;
    }
    return m ;
  }

  inline void skip_non_digit(){
    while( !valid_digit() && has_more() ) {
      p++ ;
    }
  }

  inline bool has_more(){
    return p != end ;
  }

};

#endif
