#include "colors.inc"
camera{
 location <4, 4, -5>
 look_at <0, 0, 0>
 angle 36
}
light_source{ <500, 500, -1000> White }
background { color White }

difference{
    intersection{
        box{-1, 1}
        sphere{0, 1.5}
    }
    union{
        cylinder{ <0, 0, 2>, <0, 0, -2>, 0.7}
        cylinder{ <2, 0, 0>, <-2, 0, 0>, 0.7}
        cylinder{ <0, 2, 0>, <0, -2, 0>, 0.7}
    }
    pigment{ Blue }
}
