#include <iostream>

using namespace std;

#include <openvr_mingw.hpp>

#include "shared/lodepng.h"
#include "shared/Matrices.h"
#include "shared/pathtools.h"

#define WINVER 0x500
#include <windows.h>
vr::IVRSystem *m_pHMD;
vr::IVRRenderModels *m_pRenderModels;
std::string m_strDriver;
std::string m_strDisplay;
vr::TrackedDevicePose_t m_rTrackedDevicePose[ vr::k_unMaxTrackedDeviceCount ];
Matrix4 m_rmat4DevicePose[ vr::k_unMaxTrackedDeviceCount ];

char m_rDevClassChar[ vr::k_unMaxTrackedDeviceCount ];   // for each device, a character representing its class
int main()
{

    // Loading the SteamVR Runtime
    vr::EVRInitError eError = vr::VRInitError_None;
    m_pHMD = vr::VR_Init( &eError, vr::VRApplication_Other );
    cout << "DOOM VFL VIRTUAL F***ING LOCOMOTION" << endl;
    cout << "OmarEmad version 0.0.0.3" << endl;
    cout << "SWING BOTH CONTROLLERS DOWN QUICKLY TO JUMP" << endl;
    int deadzone;
    int jumpsens;
    int haptic=0;

    cout << "Enter deadzone size for trackpad 0-75: Recommended value 25" << endl;
    cout << "Large values make the center of the trackpad inactive to allow easier teleport" << endl;
    cin>> deadzone;
    cout << "Enter Jump Threshold: Higher values require a stronger arm swing 1-5 Recommended value 1" << endl;
    cin>> jumpsens;
    cout << "Haptic feedback, 0 (battery saving) for off 1 for on (battery saving)" << endl;
    cin>> haptic;

    cout << "Running" << endl;
    //jumpsens+=1;
    float lastX=0.0f;
    INPUT ip;


    // Set up a generic keyboard event.
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0; // hardware scan code for key
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;
    INPUT input[2];
    float lastY=0.0f;
    while(1)
    {
        vr::TrackedDevicePose_t h1[ vr::k_unMaxTrackedDeviceCount ];
        vr::VRSystem()->GetDeviceToAbsoluteTrackingPose(vr::TrackingUniverseStanding, 0, h1,  vr::k_unMaxTrackedDeviceCount );
        vr::VRCompositor()->WaitGetPoses(m_rTrackedDevicePose, vr::k_unMaxTrackedDeviceCount, NULL, 0 );

        int m_iValidPoseCount = 0;
        char *m_strPoseClasses = "";

        float jumpVel =0.0f;

        //cout<<"devicew"<<vr::k_unMaxTrackedDeviceCount<<endl;
        for ( int nDevice = 0; nDevice < vr::k_unMaxTrackedDeviceCount; ++nDevice )
        {
            if ( 1 )
            {
                m_iValidPoseCount++;


                switch (m_pHMD->GetTrackedDeviceClass(nDevice))
                {
                case vr::TrackedDeviceClass_Controller:
                    m_rDevClassChar[nDevice] = 'C';
                    if(h1[nDevice].bPoseIsValid)
                        jumpVel += h1[nDevice].vVelocity.v[1];
                    break;
                case vr::TrackedDeviceClass_HMD:
                    m_rDevClassChar[nDevice] = 'H';
                    break;
                case vr::TrackedDeviceClass_Invalid:
                    m_rDevClassChar[nDevice] = 'I';
                    break;
                case vr::TrackedDeviceClass_GenericTracker:
                    m_rDevClassChar[nDevice] = 'G';
                    break;
                case vr::TrackedDeviceClass_TrackingReference:
                    m_rDevClassChar[nDevice] = 'T';
                    break;
                default:
                    m_rDevClassChar[nDevice] = '?';
                    break;
                }

                m_strPoseClasses += m_rDevClassChar[nDevice];
            }
        }

        if ( m_rTrackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].bPoseIsValid )
        {

        }

        if(  jumpVel<-jumpsens)
        {


            INPUT input[2];
            input[0].type = INPUT_KEYBOARD;
            input[0].ki.wVk = 0;
            input[0].ki.wScan = MapVirtualKey(0x20, 0);
            input[0].ki.dwFlags = KEYEVENTF_SCANCODE;
            input[0].ki.time = 0;
            input[0].ki.dwExtraInfo = 0;
            SendInput(1, input, sizeof(INPUT));
            //Sleep(5);
            input[0].type = INPUT_KEYBOARD;
            input[0].ki.wVk = 0;
            input[0].ki.wScan =MapVirtualKey(0x20, 0);
            input[0].ki.dwFlags = KEYEVENTF_SCANCODE|KEYEVENTF_KEYUP ;
            input[0].ki.time = 0;
            input[0].ki.dwExtraInfo = 0;
            SendInput(2, input, sizeof(INPUT));
            jumpVel =0.0f;

        }

// Process SteamVR events
        vr::VREvent_t event;
        while( m_pHMD->PollNextEvent( &event, sizeof( event ) ) )
        {
            //ProcessVREvent( event );
        }
        // Process SteamVR controller state
        for( vr::TrackedDeviceIndex_t unDevice = 0; unDevice < vr::k_unMaxTrackedDeviceCount; unDevice++ )
        {
            vr::VRControllerState_t state;


            if( m_pHMD->GetControllerState( unDevice, &state, sizeof(state) ) )
            {
                //m_rbShowTrackedDevice[ unDevice ] = state.ulButtonPressed == 0;


                int handID=m_pHMD->GetControllerRoleForTrackedDeviceIndex(unDevice);



                if(handID==vr::TrackedControllerRole_LeftHand)
                {
                    if(state.rAxis[0].x != lastX)

                    {

                        if(state.rAxis[0].x<-deadzone/100.0f)
                        {

                            input[0].type = INPUT_KEYBOARD;
                            input[0].ki.wVk = 0;
                            input[0].ki.wScan = MapVirtualKey(0x41, 0);
                            input[0].ki.dwFlags = KEYEVENTF_SCANCODE;
                            input[0].ki.time = 0;
                            input[0].ki.dwExtraInfo = 0;
                            SendInput(1, input, sizeof(INPUT));
                            //Sleep(5);
                            input[0].type = INPUT_KEYBOARD;
                            input[0].ki.wVk = 0;
                            input[0].ki.wScan =MapVirtualKey(0x41, 0);
                            input[0].ki.dwFlags = KEYEVENTF_SCANCODE|KEYEVENTF_KEYUP ;
                            input[0].ki.time = 0;
                            input[0].ki.dwExtraInfo = 0;
                            SendInput(2, input, sizeof(INPUT));
                            //Sleep(1/(0.1+(state.rAxis[0].x)*(state.rAxis[0].x)));
                        }
                        if(state.rAxis[0].x>deadzone/100.0f)
                        {

                            input[0].type = INPUT_KEYBOARD;
                            input[0].ki.wVk = 0;
                            input[0].ki.wScan = MapVirtualKey(0x44, 0);
                            input[0].ki.dwFlags = KEYEVENTF_SCANCODE;
                            input[0].ki.time = 0;
                            input[0].ki.dwExtraInfo = 0;
                            SendInput(1, input, sizeof(INPUT));
                            //Sleep(5);
                            input[0].type = INPUT_KEYBOARD;
                            input[0].ki.wVk = 0;
                            input[0].ki.wScan =MapVirtualKey(0x44, 0);
                            input[0].ki.dwFlags = KEYEVENTF_SCANCODE|KEYEVENTF_KEYUP ;
                            input[0].ki.time = 0;
                            input[0].ki.dwExtraInfo = 0;
                            SendInput(2, input, sizeof(INPUT));
                            //Sleep(1/(0.1+(state.rAxis[0].x)*(state.rAxis[0].x)));
                        }
                        lastX=state.rAxis[0].x;
                    }


                    if(state.rAxis[0].y != lastY)

                    {

                        if(state.rAxis[0].y<-deadzone/100.0f)
                        {
                            INPUT input[2];
                            input[0].type = INPUT_KEYBOARD;
                            input[0].ki.wVk = 0;
                            input[0].ki.wScan = MapVirtualKey(0x53, 0);
                            input[0].ki.dwFlags = KEYEVENTF_SCANCODE;
                            input[0].ki.time = 0;
                            input[0].ki.dwExtraInfo = 0;
                            SendInput(1, input, sizeof(INPUT));

                            input[0].type = INPUT_KEYBOARD;
                            input[0].ki.wVk = 0;
                            input[0].ki.wScan =MapVirtualKey(0x53, 0);
                            input[0].ki.dwFlags = KEYEVENTF_SCANCODE|KEYEVENTF_KEYUP ;
                            input[0].ki.time = 0;
                            input[0].ki.dwExtraInfo = 0;
                            SendInput(1, input, sizeof(INPUT));
                            //Sleep(5);

                            //Sleep(1/(0.1+(state.rAxis[0].y)*(state.rAxis[0].y)));
                        }
                        if(state.rAxis[0].y>deadzone/100.0f)
                        {
                            INPUT input[2];
                            input[0].type = INPUT_KEYBOARD;
                            input[0].ki.wVk = 0;
                            input[0].ki.wScan = MapVirtualKey(0x57, 0);
                            input[0].ki.dwFlags = KEYEVENTF_SCANCODE;
                            input[0].ki.time = 0;
                            input[0].ki.dwExtraInfo = 0;
                            SendInput(1, input, sizeof(INPUT));



                            input[0].type = INPUT_KEYBOARD;
                            input[0].ki.wVk = 0;
                            input[0].ki.wScan =MapVirtualKey(0x57, 0);
                            input[0].ki.dwFlags = KEYEVENTF_SCANCODE|KEYEVENTF_KEYUP ;
                            input[0].ki.time = 0;
                            input[0].ki.dwExtraInfo = 0;
                            SendInput(1, input, sizeof(INPUT));
                            //Sleep(5);

                            //Sleep(1/(0.1+(state.rAxis[0].y)*(state.rAxis[0].y)));
                        }
                        lastY=state.rAxis[0].y;
                    }



                }
                if(state.ulButtonPressed& vr::ButtonMaskFromId(vr::k_EButton_ApplicationMenu) )

                {



                }




                if(handID==vr::TrackedControllerRole_RightHand && haptic)
                {
                    if(state.ulButtonPressed& vr::ButtonMaskFromId(vr::k_EButton_Axis1) )

                    {

                        //m_pHMD->TriggerHapticPulse( unDevice, vr::EVRButtonId::k_EButton_SteamVR_Touchpad-vr::EVRButtonId::k_EButton_Axis0, 1000 );
                         m_pHMD->TriggerHapticPulse(unDevice, 0, 500);

                    }
                }

                Sleep(1);
            }
        }
    }
    return 0;
}
