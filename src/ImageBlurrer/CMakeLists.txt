add_library(ImageBlurrer ImageBlurrer.cxx)
target_include_directories(ImageBlurrer INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(ImageBlurrer PUBLIC cxx_std_11)
install(TARGETS ImageBlurrer DESTINATION lib)
install(FILES ImageBlurrer.h ImageScale.h ImageBlurrerInput.h ImageFileParser.h mRNG.h ImageBlurrerHeader.h ImageBlurrerApp.h DESTINATION include)
