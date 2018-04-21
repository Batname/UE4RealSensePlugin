/**************************************************************************************************
INTEL CORPORATION PROPRIETARY INFORMATION
This software is supplied under the terms of a license agreement or nondisclosure
agreement with Intel Corporation and may not be copied or disclosed except in
accordance with the terms of that agreement
Copyright(c) 2014-2016 Intel Corporation. All Rights Reserved.
**************************************************************************************************/

/// @file pxc3dscan.h
/// Interface definition file for PXC3DScan video module.
#pragma once

//***********************************************************************************************//
// Includes
//***********************************************************************************************//

#include "pxcsensemanager.h"

//***********************************************************************************************//
// Class definition
//***********************************************************************************************//
/// <summary>
///  3D Capture module interface definition class. 
/// </summary>
class PXC3DScan : public PXCBase
{
public:
    // Macro Functions
    // --------------------------------------------------------------------------------------------
    PXC_CUID_OVERWRITE(PXC_CUID_3D_SCAN);

    // Enumerations
    // --------------------------------------------------------------------------------------------

    /// <summary> Scanning area modes. </summary>
    enum ScanningMode
    {
        VARIABLE = 0,
        OBJECT_ON_PLANAR_SURFACE_DETECTION,
        FACE,
        HEAD,
        BODY,
    };

    /// <summary> Scanning reconstruction options (bit field). </summary>
    enum ReconstructionOption
    {
        NONE            = 0,                // No reconstruction.
        SOLIDIFICATION  = (1<<0),           // Generate a closed manifold mesh.
        TEXTURE         = (1<<1),           // Disable vertex color, and generate , 
                                            // texture map (meshBaseNameImage1.jpg), 
                                            // and material (meshBaseName.mtl) files.
        LANDMARKS       = (1<<2),           // Use face module to track and generate mesh
                                            // relative landmark data (meshBaseName.json).
        
        // Operator '|' and Operator '^' are overloaded in pxc3Dscan.cpp
    };


    /// <summary> Usability notifications. </summary>
    enum AlertEvent
    {
        // Scanning alerts (fired AFTER the system is scanning)
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // Range alerts
        ALERT_IN_RANGE = 0,
        ALERT_TOO_CLOSE,
        ALERT_TOO_FAR,

        // Tracking alerts
        ALERT_TRACKING,
        ALERT_LOST_TRACKING,

        // Pre-scanning alerts (fired BEFORE the system is scanning)
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // Each group represents a pre-conditions which must satisfied before scanning will start.

        // Tracking alerts
        // ----------------
        ALERT_SUFFICIENT_STRUCTURE,
        ALERT_INSUFFICIENT_STRUCTURE,

        // Face alerts (if ReconstructionOption::LANDMARKS is set)
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        ALERT_FACE_DETECTED,
        ALERT_FACE_NOT_DETECTED,

        ALERT_FACE_X_IN_RANGE,
        ALERT_FACE_X_TOO_FAR_LEFT,
        ALERT_FACE_X_TOO_FAR_RIGHT,

        ALERT_FACE_Y_IN_RANGE,
        ALERT_FACE_Y_TOO_FAR_UP,
        ALERT_FACE_Y_TOO_FAR_DOWN,

        ALERT_FACE_Z_IN_RANGE,
        ALERT_FACE_Z_TOO_CLOSE,
        ALERT_FACE_Z_TOO_FAR,

        ALERT_FACE_YAW_IN_RANGE,
        ALERT_FACE_YAW_TOO_FAR_LEFT,
        ALERT_FACE_YAW_TOO_FAR_RIGHT,

        ALERT_FACE_PITCH_IN_RANGE,
        ALERT_FACE_PITCH_TOO_FAR_UP,
        ALERT_FACE_PITCH_TOO_FAR_DOWN,

        ALERT_FACE_MOTION_TOO_SLOW,
        ALERT_FACE_MOTION_TOO_FAST,
        ALERT_FACE_MOTION_IN_RANGE,

        // Fiducial Marker Tracking Detected.
        ALERT_FIDUCIAL_MARKER_DETECTED,
        ALERT_FIDUCIAL_MARKER_NOT_DETECTED,
    };

    /// <summary> Output mesh formats. </summary>
    enum FileFormat
    {
        OBJ = 0,
        PLY,
        STL
    };

    // Structs
    // --------------------------------------------------------------------------------------------

    /// <summary> Scanning area properties. </summary>
    struct Area
    {
        PXCSize3DF32  shape;                // Scanning volume size (w,h,d) in meters.
        pxcI32        resolution;           // Voxel resolution (along longest shape axis).
        pxcI32        reserved[64];         // Reserved structure.
    };

    /// <summary> Scanning configuration. </summary>
    struct Configuration
    {
        pxcBool              startScan;                 // Start the scan.
        ScanningMode         mode;                      // Scanning mode.
        ReconstructionOption options;                   // Options for reconstruction.
        
        // New configuration reserved objects.
        pxcI32               maxTriangles;              // Number of triangles.
        pxcI32               maxVertices;               // Number of vertices.
        PXCSizeI32           maxTextureResolution;      // (w,h) in pixels.
        pxcBool              flopPreviewImage;          // https://en.wikipedia.org/wiki/Flopped_image
        pxcBool              useMarker;                 // Tracking aid.
        pxcI32               reserved[59];              // Reserved bits.
    };

    /// <summary> Usability notifications data. </summary>
    struct AlertData
    {
        pxcI64     timeStamp;
        AlertEvent label;
        pxcI32     reserved[5];
    };

    // Nested class definitions
    // --------------------------------------------------------------------------------------------

    /// <summary> User defined handler for alert processing. </summary>
    class AlertHandler
    {
    public:
        virtual void PXCAPI OnAlert(const AlertData& data) = 0;
    };

    // Public Member Functions
    //---------------------------------------------------------------------------------------------

    /// <summary>
    /// Reconfigure the scanning area according to the provided values and restart the scanning
    /// process if successful.
    /// </summary>  
    /// <param name="area"> The Area set for scanning. </param>
    /// <returns> 
    /// Returns the PXCStatus error status.
    /// Returns error if called when PXC3DScan::Configuration.mode is not set to VARIABLE.
    /// </returns>
    virtual pxcStatus PXCAPI SetArea(Area area) = 0;

    /// <summary> Get a copy of the current scanning area. </summary>
    /// <returns> The specified area of the scan.</returns>
    virtual Area PXCAPI QueryArea() = 0;

    /// <summary>
    /// Reconfigure the scanning configuration according to the provided values and,
    /// if successful, restart the scanning process.
    /// </summary>
    /// <param name="config"> The Configuration setting for scanning. </param>
    /// <returns> Returns the PXCStatus error status. </returns>
    virtual pxcStatus PXCAPI SetConfiguration(Configuration config) = 0;

    /// <summary> Get a copy of the current configuration. </summary>
    /// <returns> Returns the specified configuration. </returns>
    virtual Configuration PXCAPI QueryConfiguration() = 0;

    /// <summary> 
    /// Allocate and return a rendered preview image to show to the user as visual feedback. 
    /// The image, which is available before and after the system is scanning, is rendered from the
    /// perspective of the most recently processed frame. The size of returned image depends on the
    /// (color, depth) profile. For any one profile, the size of the returned image is different
    /// before & after the system is scanning.Call Release to deallocate the returned image object.
    /// </summary>
    /// <returns> Pointer to the preview image. </returns>
    virtual PXCImage* PXCAPI AcquirePreviewImage() = 0;

    /// <summary>
    /// Return the extent of the visible object (in the preview image)
    /// in normalized image space coordinates (i.e. 0.0 - 1.0).
    /// </summary>
    /// <returns> The bounding box rectangle of the visible object. </returns>
    virtual PXCRectF32 PXCAPI QueryBoundingBox() = 0;

    /// <summary>
    /// Determine if the scan has started.
    /// Some scanning modes implement pre-conditions which can delay the start.
    /// </summary>
    /// <returns> Boolean whether the scan has started or not. </returns>
    virtual pxcBool PXCAPI IsScanning(void) = 0;

    /// <summary>
    /// Generate a mesh from the current scanned data.
    /// If TEXTURE is enabled, additional files are generated (i.e. .mtl, .jpg).
    /// If LANDMARKS is enabled, mesh relative landmark data is generated (i.e. .json).
    /// </summary>
    /// <param name= "format"> Output file format. </param>
    /// <param name= "filename"> Output file name. </param>
    /// <returns>
    /// Returns the PXCStatus error status.
    /// Returns an error if not scanning. On success, this function resets the scanning system. 
    /// (e.g. SetConfiguration(QueryConfiguration())).
    /// </returns>
    virtual pxcStatus PXCAPI Reconstruct(FileFormat format, const pxcCHAR* fileName) = 0;

    /// <summary>
    /// Optionally register to receive event notifications. A subsequent call will replace the 
    /// previously registered handler object. Subscribe(NULL) to unregister.
    /// </summary>
    /// <param name= "handler"> The alert handler. </param>
    virtual void PXCAPI Subscribe(PXC3DScan::AlertHandler* handler) = 0;

    /// <summary> File extension helper. </summary>
    /// <param name="format"> the file format. </param>
    __inline static const pxcCHAR * FileFormatToString(FileFormat format) {
        switch (format) {
        case OBJ: return (const pxcCHAR*)L"obj";
        case PLY: return (const pxcCHAR*)L"ply";
        case STL: return (const pxcCHAR*)L"stl";
        }
        return (const pxcCHAR*)L"Unknown";
    }
};

/// <summary> Scanning reconstruction options (bit field) composition helper. </summary>
// function Overloading operators for enumerated data type PXC3DScan::ReconstructionOption.
//***********************************************************************************************//

inline static PXC3DScan::ReconstructionOption operator|(    PXC3DScan::ReconstructionOption lhs, 
                                                            PXC3DScan::ReconstructionOption rhs
                                                        )
{
    return (PXC3DScan::ReconstructionOption)(static_cast<int>(lhs) | static_cast<int>(rhs));
}

//***********************************************************************************************//

/// <summary> Scanning reconstruction options (bit field) composition helper. </summary>
inline static PXC3DScan::ReconstructionOption operator^(    PXC3DScan::ReconstructionOption lhs, 
                                                            PXC3DScan::ReconstructionOption rhs
                                                        )
{
    return (PXC3DScan::ReconstructionOption)(static_cast<int>(lhs) ^ static_cast<int>(rhs));
}

//***********************************************************************************************//