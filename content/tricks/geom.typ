== geom.h
```cpp
Basic Utilities
EPS-based floating point comparisons (sign).
Square function (sq).

2D Points / Vectors (struct P)
Vector arithmetic (+, -, *, /).
Dot and Cross products.
Distance calculations (dist, dist2).
Rotation (rotate) and Perpendicular vector (perp).
Unit vector normalization (unit).
Comparators for sorting (<, ==) and output stream support (<<).

Lines (struct Line)Defined as $ax + by = c$.
Parallel check.
Intersection with another line.
Distance from a point to the line.
Orthogonal projection of a point onto the line.
Reflection of a point across the line.

Segments (struct Seg)
Check if a point is strictly on the segment.
Shortest distance from a point to the segment (clamped to endpoints).
PolygonsSigned area.
Convexity check.
Point in polygon test (returns: inside, outside, or on boundary).
Convex Hull (Monotone Chain algorithm).

Circles (struct Circle)
Check if a point is inside or on the boundary.
Intersection with a line (finds 0, 1, or 2 points)

```