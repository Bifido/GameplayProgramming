#pragma once

#include "DynamicRenderable.h"
#include "..\..\Common\Utilities.h"

class DynamicLines : public DynamicRenderable
{
  typedef Ogre::Camera Camera;
  typedef Ogre::RenderOperation::OperationType OperationType;

public:
/// Constructor - see setOperationType() for description of argument.
  DynamicLines(OperationType opType = Ogre::RenderOperation::OT_LINE_STRIP);
  virtual ~DynamicLines();

  enum {
	  POSITION_BINDING,
	  TEXCOORD_BINDING
  };

  /// Add a point to the point list
  void addPoint(const Vec3 &p);
  /// Add a point to the point list
  void addPoint(real x, real y, real z);

  /// Change the location of an existing point in the point list
  void setPoint(uint8 index, const Vec3 &value);

  /// Return the location of an existing point in the point list
  const Vec3& getPoint(uint8 index) const;

  /// Return the total number of points in the point list
  uint8 getNumPoints(void) const;

  /// Remove all points from the point list
  void clear();

  /// Call this to update the hardware buffer after making changes.
  void update();

  /** Set the type of operation to draw with.
   * @param opType Can be one of
   *    - RenderOperation::OT_LINE_STRIP
   *    - RenderOperation::OT_LINE_LIST
   *    - RenderOperation::OT_POINT_LIST
   *    - RenderOperation::OT_TRIANGLE_LIST
   *    - RenderOperation::OT_TRIANGLE_STRIP
   *    - RenderOperation::OT_TRIANGLE_FAN
   *    The default is OT_LINE_STRIP.
   */
  void setOperationType(OperationType opType);
  OperationType getOperationType() const;

protected:
  /// Implementation DynamicRenderable, creates a simple vertex-only decl
  virtual void createVertexDeclaration();
  /// Implementation DynamicRenderable, pushes point list out to hardware memory
  virtual void fillHardwareBuffers();

private:
  MGDVector<Vec3> m_Points;
  bool m_bDirty;
};