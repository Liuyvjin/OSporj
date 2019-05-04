# proj2
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(porj2)
ADD_EXECUTABLE(MergesortSingle MergesortSingle.c)
ADD_EXECUTABLE(MergesortMulti MergesortMulti.c)
TARGET_LINK_LIBRARIES(MergesortMulti pthread)
ADD_EXECUTABLE(BBC BurgerBuddies.c)
TARGET_LINK_LIBRARIES(BBC pthread)