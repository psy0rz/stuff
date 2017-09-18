module trap(base,top,height, depth)
{

    polyhedron(
        points=[
            //front
            [0,0,0],
            [base,0,0],
            [(base/2)+(top/2),height,0],
            [(base/2)-(top/2),height,0],

            //rear
            [0,0,depth],
            [base,0,depth],
            [(base/2)+(top/2),height,depth],
            [(base/2)-(top/2),height,depth],
        ],

        faces=[
            [0,1,2,3], //front
            [7,6,5,4], //rear
            [0,4,5,1], //bottom
            [2,6,7,3], //top
            [0,3,7,4], //left
            [5,6,2,1], //right
            
        ]

    );
}

trap(20,5,5,20);
