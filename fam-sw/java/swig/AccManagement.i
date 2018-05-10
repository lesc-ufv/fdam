%module fam
%{
    #include "fam/AccManagement.h"
    #include "fam/Accelerator.h"
%}
%include "typemaps.i"
%apply signed char *INOUT{signed char * data};
%apply short *INOUT{short * data};
%apply int *INOUT{int * data};
%apply long *INOUT{long * data};

%apply signed char *INOUT{const signed char * data};
%apply short *INOUT{const short * data};
%apply int *INOUT{const int * data};
%apply long *INOUT{const long * data};
%apply long long *INOUT{long long * data};

%include "fam/AccManagement.h"
%include "fam/Accelerator.h"

%pragma(java) jniclassimports=%{

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.nio.file.Files;
import java.util.logging.Level;
import java.util.logging.Logger;
    
%}

%pragma(java) jniclasscode=%{

  static {
      try{
            String libName = "libjava_fam.so";
            URL url = famJNI.class.getResource("/" + libName);
            File tmpDir = Files.createTempDirectory("lib").toFile();
            tmpDir.deleteOnExit();
            File nativeLibTmpFile = new File(tmpDir, libName);
            nativeLibTmpFile.deleteOnExit();
            try (InputStream in = url.openStream()) {
                Files.copy(in, nativeLibTmpFile.toPath());
            }
            System.load(nativeLibTmpFile.getAbsolutePath());
      } catch (IOException ex) {
          Logger.getLogger(famJNI.class.getName()).log(Level.SEVERE, null, ex);
      }
  }
    
%}
