#include <list> 
#include <string>
#include <iostream>

#include <Magick++.h> 

#include "mosaic_options_file_reader.hpp"

using namespace std;
using namespace Magick;

/// \brief Create a horizontal mosaic of two images.

int
main
(int    argc,
 char** argv)
{
  {
    try
    {
      //
      // Initialize Magick++
      // Do this in the first place so it is possible to
      // declare objects defined by this library.
      //

      InitializeMagick(*argv);

      // Declare our variables and objects.

      Image                      image_left;
      Image                      image_right;
      list<Image>                list_of_images;
      Image                      mosaic;
      string                     name_image_left;
      string                     name_image_right;
      string                     name_mosaic;
      string                     name_options_file;
      mosaic_options_file_reader op_reader;
      mosaic_options             options;
      int                        status;

      //
      // Check that we've got our unique parameter (the
      // name of the options file)
      //

      if (argc < 2) return 1; // No options file name in command line.
    
      // Read the options controlling our behaviour.

      name_options_file = argv[1];
      status = op_reader.parse_file(name_options_file, options);

      if (status != 0) return 2; // Error reading the options file.

      name_image_left  = options.left_image_file_name;
      name_image_right = options.right_image_filename;
      name_mosaic      = options.mosaic_file_name;

      // Read the images.

      image_left.read(name_image_left);

      image_right.read(name_image_right);

      // Create the list of images.

      list_of_images.push_back(image_left);
      list_of_images.push_back(image_right);

      // Create the mosaic.

      appendImages(&mosaic, list_of_images.begin(), list_of_images.end());

      // Write the mosaic to disk.

      mosaic.write(name_mosaic);

      // That's all.

      return 0; // This error codes says "everything went right".
    }
    catch (...)
    {
       return 3; // Error when processing the image(s).
    }
  }
}
