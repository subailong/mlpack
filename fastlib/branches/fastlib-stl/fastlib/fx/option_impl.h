#ifndef MLPACK_IO_OPTION_IMPL_H
#define MLPACK_IO_OPTION_IMPL_H

template<typename N>
Option<N>::Option(bool ignoreTemplate,
                N defaultValue,
                const char* identifier,
                const char* description,
                const char* parent,
                bool required) {
  if (ignoreTemplate)
    IO::Add(identifier, description, parent, required);
  else {
    IO::Add<N>(identifier, description, parent, required);

    //Create the full pathname.
    std::string pathname = IO::SanitizeString(parent) + std::string(identifier);
    IO::GetValue<N>(pathname.c_str()) = defaultValue;
  }
}

#endif 
