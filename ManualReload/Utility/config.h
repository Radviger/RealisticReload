#pragma once
#include <sstream>

class CConfig {
  public:
    CConfig() { }

    explicit CConfig( const char* fileName ) : CConfig( fileName, false ) { }

    explicit CConfig( const char* fileName, bool customPath ) {

        auto path = customPath ? fileName :
                    Utility::GetWorkingDirectory() + "\\" + fileName;
        strcpy_s( filename, &path[0] );
    }

    bool getText( char * outBuffer, const char * section, const char * key ) const {
        GetPrivateProfileString( TEXT( section ),
                                 TEXT( key ),
                                 NULL,
                                 outBuffer,
                                 sizeof( outBuffer ),
                                 TEXT( filename ) );

        return outBuffer &&
               GetLastError() == ERROR_SUCCESS;
    }

    void setText( const char * section, const char * key, const char * value ) const {
        WritePrivateProfileString( TEXT( section ),
                                   TEXT( key ),
                                   TEXT( value ),
                                   filename );
    }

    template <typename T>
    T get( const char * section, const char * key, T defaultValue ) {
        T result{};

        TCHAR inBuf[80];

        if ( !getText( inBuf, section, key ) )
            return defaultValue;

        std::stringstream sstream;

        sstream.imbue( std::locale( "en-us" ) );

        if ( typeid( T ) == typeid( bool ) ) {
            sstream << std::boolalpha << inBuf;
        }

        else {
            sstream << inBuf;
        }

        sstream >> result;

        return result;
    }

    template <typename T>
    void set( const char * section, const char * key, T val ) {
        std::stringstream sstream;

        sstream.imbue( std::locale( "en-us" ) );

        if ( typeid( T ) == typeid( bool ) ) {
            sstream << std::boolalpha << val;
        }

        else {
            sstream << val;
        }

        setText( section, key, &sstream.str()[0] );
    }

    TCHAR filename[MAX_PATH];
};
