compiler = gcc
flags = --pedantic-errors -Wall -Werror -fPIC -std=c99
objects = flub.o


# Version information.
major_number = 0
minor_number = 0
release_number = 1
version = ${major_number}.${minor_number}.${release_number}

# Library name.
name = flub
linker_name = lib${name}.so
soname = ${linker_name}.${major_number}
real_name = ${linker_name}.${version}

# Library paths.
base_path = /usr/
include_path = ${base_path}include/
library_path = ${base_path}lib/


# Perform default functionality.
default: create tidy done

# Install the library.
install: install_actual done


# Clean the directory.
clean: tidy
	@echo "Cleaning."
	@rm -f ${real_name}


# Compile the library.
.c.o:
	${compiler} ${flags} -c $<


# Create the library.
create: ${objects}
	${compiler} -shared -Wl,-soname,${soname} -o ${real_name} *.o


# Print the conclusion.
done:
	@echo "Done."


# Install the library.
install_actual:
	@echo "Installing."
	@# Install library files.
	@mv ${real_name} ${library_path}
	@ln -sf ${library_path}${real_name} ${library_path}${linker_name}
	@ln -sf ${library_path}${real_name} ${library_path}${soname}
	@# Make header files readable to all.
	@chmod 0744 ${include_path}*.h
	@# Install header files.
	@cp -f *.h ${include_path}


# Tidy the build directory.
tidy:
	@echo "Tidying."
	@rm -f *.o
