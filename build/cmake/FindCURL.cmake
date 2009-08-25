# - Find curl
# Find the native CURL headers and libraries.
#
#  CURL_INCLUDE_DIRS - where to find curl/curl.h, etc.
#  CURL_LIBRARIES    - List of libraries when using curl.
#  CURL_FOUND        - True if curl found.


SET (SEARCH_CURL CACHE BOOL "Sould we search for Curl ?" )
SET (SEARCH_CURL_LIB_PATH CACHE PATH "Path to the CURL libs (empty for unix systems)")

IF (SEARCH_CURL)

	IF (WIN32)
		FIND_PATH(CURL_INCLUDE_DIRS curl/curl.h
  				${SEARCH_CURL_LIB_PATH}/include 
  				${SEARCH_CURL_LIB_PATH}
  				NO_DEFAULT_PATH)
  		
   		FIND_LIBRARY(CURL_LIBRARIES
	  		libcurl.lib
	  		PATH ${SEARCH_CURL_LIB_PATH} ${SEARCH_CURL_LIB_PATH} NO_DEFAULT_PATH)
	
	# if unix / mac and search geos
	ELSE (WIN32)
		
		FIND_PATH(CURL_INCLUDE_DIRS curl/curl.h
  				${SEARCH_CURL_LIB_PATH}/include
  				${SEARCH_CURL_LIB_PATH})
  		
  		FIND_LIBRARY(CURL_LIBRARIES
	  		curl
	  		PATH ${SEARCH_CURL_LIB_PATH}/lib	${SEARCH_CURL_LIB_PATH})
  		  		  			
	ENDIF (WIN32)
	
	
	
	
	
	MARK_AS_ADVANCED(
				CURL_INCLUDE_DIRS
				CURL_LIBRARIES
					)
  
  
  ## IF GEOS PARAMETERS ARE DEFINED, USE THEM
   IF(CURL_INCLUDE_DIRS)
    INCLUDE_DIRECTORIES("${CURL_INCLUDE_DIRS}")
    #INCLUDE_DIRECTORIES("/usr/test/test/test")
    ##INCLUDE_DIRECTORIES(${SEARCH_GEOS_LIB_PATH}/source/headers)
    MESSAGE ("DBG : CURL include is ${CURL_INCLUDE_DIRS}")
  ENDIF(CURL_INCLUDE_DIRS)
 
 
  IF(CURL_LIBRARIES)
    LINK_LIBRARIES(${CURL_LIBRARIES})
    MESSAGE ("DBG : CURL lib is ${CURL_LIBRARIES}")
  ENDIF(CURL_LIBRARIES)


ENDIF(SEARCH_CURL)