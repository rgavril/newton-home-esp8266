#include "Logging.h"

void Logging::begin(int level, Print* logOutput, bool showLevel){
    _level     = constrain(level,LOG_LEVEL_SILENT,LOG_LEVEL_VERBOSE);
	_showLevel = showLevel;
    _logOutput = logOutput;
}

void Logging::setPrefix(printfunction f){
    _prefix = f;
}

void Logging::setSuffix(printfunction f){
    _suffix = f;
}

void Logging::print(const __FlashStringHelper *format, va_list args) {
#ifndef DISABLE_LOGGING	  	
  PGM_P p = reinterpret_cast<PGM_P>(format);
  char c = pgm_read_byte(p++);
  for(;c != 0; c = pgm_read_byte(p++)){
    if (c == '%') {
      c = pgm_read_byte(p++);
      printFormat(c, &args);
    } else {
      _logOutput->print(c);
    }
  }
#endif	  
}

void Logging::print(const char *format, va_list args) {
#ifndef DISABLE_LOGGING	  	
  for (; *format != 0; ++format) {
    if (*format == '%') {
      ++format;
      printFormat(*format, &args);
    } else {
      _logOutput->print(*format);
    }
  }
#endif	  
}

void Logging::printFormat(const char format, va_list *args) {
#ifndef DISABLE_LOGGING	  	
  if (format == '\0') return;

  if (format == '%') {
    _logOutput->print(format);
    return;
  }

  if( format == 's' ) {
    register char *s = (char *)va_arg( *args, int );
    _logOutput->print(s);
    return;
  }

  if( format == 'd' || format == 'i') {
    _logOutput->print(va_arg( *args, int ),DEC);
    return;
  }

  if( format == 'D' || format == 'F') {
    _logOutput->print(va_arg( *args, double ));
    return;
  }

  if( format == 'x' ) {
    _logOutput->print(va_arg( *args, int ),HEX);
    return;
  }

  if( format == 'X' ) {
    _logOutput->print("0x");
    _logOutput->print(va_arg( *args, int ),HEX);
    return;
  }

  if( format == 'b' ) {
    _logOutput->print(va_arg( *args, int ),BIN);
    return;
  }

  if( format == 'B' ) {
    _logOutput->print("0b");
    _logOutput->print(va_arg( *args, int ),BIN);
    return;
  }

  if( format == 'l' ) {
    _logOutput->print(va_arg( *args, long ),DEC);
    return;
  }

  if( format == 'c' ) {
    _logOutput->print((char) va_arg( *args, int ));
    return;
  }

  if( format == 't' ) {
    if (va_arg( *args, int ) == 1) {
      _logOutput->print("T");
    }
    else {
      _logOutput->print("F");
    }
    return;
  }

  if( format == 'T' ) {
    if (va_arg( *args, int ) == 1) {
      _logOutput->print(F("true"));
    }
    else {
      _logOutput->print(F("false"));
    }
    return;
  }
#endif	  
}
 
Logging Log = Logging();
