%module fam_ase
%{
    #include "AccManagement.h"
    #include "Accelerator.h"
%}

%include "AccManagement.h"
%include "Accelerator.h"

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
            String libName = "libjava_fam_ase.so";
            URL url = fam_aseJNI.class.getResource("/" + libName);
            File tmpDir = Files.createTempDirectory("lib").toFile();
            tmpDir.deleteOnExit();
            File nativeLibTmpFile = new File(tmpDir, libName);
            nativeLibTmpFile.deleteOnExit();
            try (InputStream in = url.openStream()) {
                Files.copy(in, nativeLibTmpFile.toPath());
            }
            System.load(nativeLibTmpFile.getAbsolutePath());
      } catch (IOException ex) {
          Logger.getLogger(fam_aseJNI.class.getName()).log(Level.SEVERE, null, ex);
      }
  }
    
%}
