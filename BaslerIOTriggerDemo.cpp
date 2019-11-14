// Simple demo to grab camera frames when triggered by camera's digital input

#include <iostream>

#define GENICAM_USER_ALWAYS_LINK_RELEASE
#include <pylon/PylonIncludes.h>
#include <pylon/usb/BaslerUsbInstantCamera.h>

using namespace Pylon;

int main()
{
   try
   {
      PylonAutoInitTerm pylonAutoInitTerm; // library init/shutdown
      CBaslerUsbInstantCamera camera( CTlFactory::GetInstance().CreateFirstDevice());
      std::cout << "Using device " << camera.GetDeviceInfo().GetModelName() << std::endl;

      // see:
      // https://docs.baslerweb.com/trigger-mode.html
      // https://docs.baslerweb.com/triggered-image-acquisition.html
      // https://docs.baslerweb.com/trigger-source.html#setting-a-hardware-trigger-source
      
      camera.TriggerSelector.SetValue(Basler_UsbCameraParams::TriggerSelector_FrameStart);
      camera.TriggerSource.SetValue(Basler_UsbCameraParams::TriggerSource_Line1);
      camera.TriggerActivation.SetValue(Basler_UsbCameraParams::TriggerActivation_RisingEdge);
      camera.TriggerDelay.SetValue(0.0); // unit is microseconds
      camera.LineDebouncerTime.SetValue(10.0); // unit is microseconds
      camera.TriggerMode.SetValue(Basler_UsbCameraParams::TriggerMode_On);

      camera.StartGrabbing( 100);
      // This smart pointer will receive the grab result data.
      CGrabResultPtr ptrGrabResult;
      unsigned grabCounter = 0;
      while ( camera.IsGrabbing())
      {
         // Wait for an image and then retrieve it. A timeout of 5000 ms is used.
         const int timeoutMs = 5000;
         camera.RetrieveResult( timeoutMs, ptrGrabResult, TimeoutHandling_ThrowException);

         if (ptrGrabResult->GrabSucceeded())
            std::cout << "Grabbed " << ++grabCounter << std::endl;
         else
            std::cout << "Error: " << ptrGrabResult->GetErrorCode() << " " << ptrGrabResult->GetErrorDescription() << std::endl;
      }
   }
   catch (const GenericException &e)
   {
      // Error handling.
      std::cerr << "An exception occurred." << std::endl
            << e.GetDescription() << std::endl;
      return 1;
   }
   return 0;
}
