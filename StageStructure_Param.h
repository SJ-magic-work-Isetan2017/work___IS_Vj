/************************************************************
************************************************************/
static const float OptimumDistance = 623;
static const float CamAngle = 60;

/************************************************************
************************************************************/
const float offset_Layer = 700;
static STAGE_CAM_POSE Stage_Cam_Pose__Layer[] = {
	{ // 0
		ofVec3f(0, 0, -offset_Layer * 0), ofVec3f(0, 1, 0), 0,
		{ // cam
			ofVec3f(0, 0, -offset_Layer * 0 + OptimumDistance), // pos
			ofVec3f(0, 0, -offset_Layer * 0), // LookAt
			ofVec3f(0, 1, 0), // Upper
			CamAngle,
		},
	},
	{ // 1
		ofVec3f(0, 0, -offset_Layer * 1), ofVec3f(0, 1, 0), 0,
		{ // cam
			ofVec3f(0, 0, -offset_Layer * 1 + OptimumDistance), // pos
			ofVec3f(0, 0, -offset_Layer * 1), // LookAt
			ofVec3f(0, 1, 0), // Upper
			CamAngle,
		},
	},
	{ // 2
		ofVec3f(0, 0, -offset_Layer * 2), ofVec3f(0, 1, 0), 0,
		{ // cam
			ofVec3f(0, 0, -offset_Layer * 2 + OptimumDistance), // pos
			ofVec3f(0, 0, -offset_Layer * 2), // LookAt
			ofVec3f(0, 1, 0), // Upper
			CamAngle,
		},
	},
	{ // 3
		ofVec3f(0, 0, -offset_Layer * 3), ofVec3f(0, 1, 0), 0,
		{ // cam
			ofVec3f(0, 0, -offset_Layer * 3 + OptimumDistance), // pos
			ofVec3f(0, 0, -offset_Layer * 3), // LookAt
			ofVec3f(0, 1, 0), // Upper
			CamAngle,
		},
	},
	{ // 4
		ofVec3f(0, 0, -offset_Layer * 4), ofVec3f(0, 1, 0), 0,
		{ // cam
			ofVec3f(0, 0, -offset_Layer * 4 + OptimumDistance), // pos
			ofVec3f(0, 0, -offset_Layer * 4), // LookAt
			ofVec3f(0, 1, 0), // Upper
			CamAngle,
		},
	},
	{ // 5
		ofVec3f(0, 0, -offset_Layer * 5), ofVec3f(0, 1, 0), 0,
		{ // cam
			ofVec3f(0, 0, -offset_Layer * 5 + OptimumDistance), // pos
			ofVec3f(0, 0, -offset_Layer * 5), // LookAt
			ofVec3f(0, 1, 0), // Upper
			CamAngle,
		},
	},
};

/************************************************************
************************************************************/
ofVec3f ofsVec_x(WIDTH, 0, 0);
ofVec3f ofsVec_y(0, HEIGHT, 0);
ofVec3f OptimumDistance_z(0, 0, OptimumDistance);

static STAGE_CAM_POSE Stage_Cam_Pose__Surface[] = {
	{ // 0
		ofVec3f(0, 0, 0) + ofsVec_x * 0 + ofsVec_y * 0, ofVec3f(0, 1, 0), 0,
		{ // cam
			ofVec3f(0, 0, 0) + ofsVec_x * 0 + ofsVec_y * 0 + OptimumDistance_z, // pos
			ofVec3f(0, 0, 0) + ofsVec_x * 0 + ofsVec_y * 0, // LookAt
			ofVec3f(0, 1, 0), // Upper
			CamAngle,
		},
	},
	{ // 1
		ofVec3f(0, 0, 0) + ofsVec_x * 0 + ofsVec_y * (-1), ofVec3f(0, 1, 0), 0,
		{ // cam
			ofVec3f(0, 0, 0) + ofsVec_x * 0 + ofsVec_y * (-1) + OptimumDistance_z, // pos
			ofVec3f(0, 0, 0) + ofsVec_x * 0 + ofsVec_y * (-1), // LookAt
			ofVec3f(0, 1, 0), // Upper
			CamAngle,
		},
	},
	{ // 2
		ofVec3f(0, 0, 0) + ofsVec_x * (-1) + ofsVec_y * (-1), ofVec3f(0, 1, 0), 0,
		{ // cam
			ofVec3f(0, 0, 0) + ofsVec_x * (-1) + ofsVec_y * (-1) + OptimumDistance_z, // pos
			ofVec3f(0, 0, 0) + ofsVec_x * (-1) + ofsVec_y * (-1), // LookAt
			ofVec3f(0, 1, 0), // Upper
			CamAngle,
		},
	},
	{ // 3
		ofVec3f(0, 0, 0) + ofsVec_x * (-1) + ofsVec_y * 0, ofVec3f(0, 1, 0), 0,
		{ // cam
			ofVec3f(0, 0, 0) + ofsVec_x * (-1) + ofsVec_y * 0 + OptimumDistance_z, // pos
			ofVec3f(0, 0, 0) + ofsVec_x * (-1) + ofsVec_y * 0, // LookAt
			ofVec3f(0, 1, 0), // Upper
			CamAngle,
		},
	},
	{ // 4
		ofVec3f(0, 0, 0) + ofsVec_x * (-1) + ofsVec_y * 1, ofVec3f(0, 1, 0), 0,
		{ // cam
			ofVec3f(0, 0, 0) + ofsVec_x * (-1) + ofsVec_y * 1 + OptimumDistance_z, // pos
			ofVec3f(0, 0, 0) + ofsVec_x * (-1) + ofsVec_y * 1, // LookAt
			ofVec3f(0, 1, 0), // Upper
			CamAngle,
		},
	},
	{ // 5
		ofVec3f(0, 0, 0) + ofsVec_x * 0 + ofsVec_y * 1, ofVec3f(0, 1, 0), 0,
		{ // cam
			ofVec3f(0, 0, 0) + ofsVec_x * 0 + ofsVec_y * 1 + OptimumDistance_z, // pos
			ofVec3f(0, 0, 0) + ofsVec_x * 0 + ofsVec_y * 1, // LookAt
			ofVec3f(0, 1, 0), // Upper
			CamAngle,
		},
	},
	{ // 6
		ofVec3f(0, 0, 0) + ofsVec_x * 1 + ofsVec_y * 1, ofVec3f(0, 1, 0), 0,
		{ // cam
			ofVec3f(0, 0, 0) + ofsVec_x * 1 + ofsVec_y * 1 + OptimumDistance_z, // pos
			ofVec3f(0, 0, 0) + ofsVec_x * 1 + ofsVec_y * 1, // LookAt
			ofVec3f(0, 1, 0), // Upper
			CamAngle,
		},
	},
	{ // 7
		ofVec3f(0, 0, 0) + ofsVec_x * 1 + ofsVec_y * 0, ofVec3f(0, 1, 0), 0,
		{ // cam
			ofVec3f(0, 0, 0) + ofsVec_x * 1 + ofsVec_y * 0 + OptimumDistance_z, // pos
			ofVec3f(0, 0, 0) + ofsVec_x * 1 + ofsVec_y * 0, // LookAt
			ofVec3f(0, 1, 0), // Upper
			CamAngle,
		},
	},
	{ // 8
		ofVec3f(0, 0, 0) + ofsVec_x * 1 + ofsVec_y * (-1), ofVec3f(0, 1, 0), 0,
		{ // cam
			ofVec3f(0, 0, 0) + ofsVec_x * 1 + ofsVec_y * (-1) + OptimumDistance_z, // pos
			ofVec3f(0, 0, 0) + ofsVec_x * 1 + ofsVec_y * (-1), // LookAt
			ofVec3f(0, 1, 0), // Upper
			CamAngle,
		},
	},
};

/************************************************************
************************************************************/
static STAGE_CAM_POSE Stage_Cam_Pose__Cube[] = {
	{ // 0
		ofVec3f(0, 0, 0), ofVec3f(0, 1, 0), 0,
		{ // cam
			ofVec3f(0, 0, OptimumDistance), // pos
			ofVec3f(0, 0, 0), // LookAt
			ofVec3f(0, 1, 0), // Upper
			CamAngle,
		},
	},
	{ // 1
		ofVec3f(0, 0, WIDTH), ofVec3f(0, 1, 0), 180,
		{ // cam
			ofVec3f(0, 0, WIDTH - OptimumDistance), // pos
			ofVec3f(0, 0, WIDTH), // LookAt
			ofVec3f(0, 1, 0), // Upper
			CamAngle,
		},
	},
	{ // 2
		ofVec3f(-WIDTH/2, 0, WIDTH/2), ofVec3f(0, 1, 0), 90,
		{ // cam
			ofVec3f(-WIDTH/2 + OptimumDistance, 0, WIDTH/2), // pos
			ofVec3f(-WIDTH/2, 0, WIDTH/2), // LookAt
			ofVec3f(0, 1, 0), // Upper
			CamAngle,
		},
	},
	{ // 3
		ofVec3f(WIDTH/2, 0, WIDTH/2), ofVec3f(0, 1, 0), -90,
		{ // cam
			ofVec3f(WIDTH/2 - OptimumDistance, 0, WIDTH/2), // pos
			ofVec3f(WIDTH/2, 0, WIDTH/2), // LookAt
			ofVec3f(0, 1, 0), // Upper
			CamAngle,
		},
	},
	{ // 4
		ofVec3f(0, HEIGHT/2, HEIGHT/2), ofVec3f(1, 0, 0), 90,
		{ // cam
			ofVec3f(0, HEIGHT/2 - OptimumDistance, HEIGHT/2), // pos
			ofVec3f(0, HEIGHT/2, HEIGHT/2), // LookAt
			ofVec3f(0, 0, 1), // Upper
			CamAngle,
		},
	},
	{ // 5
		ofVec3f(0, -HEIGHT/2, HEIGHT/2), ofVec3f(1, 0, 0), -90,
		{ // cam
			ofVec3f(0, -HEIGHT/2 + OptimumDistance, HEIGHT/2), // pos
			ofVec3f(0, -HEIGHT/2, HEIGHT/2), // LookAt
			ofVec3f(0, 0, -1), // Upper
			CamAngle,
		},
	},
};


