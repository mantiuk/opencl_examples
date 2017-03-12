__kernel void animate(__global float * pos, __global float * vel, __global float * p_force, float attract_x, float attract_y, float dt, int n)
{
	int idx = get_global_id ( 0 );

	if ( idx >= n )
		return;

	idx *= 3;
	
	float3 boxSize = (float3)(1.0f, 1.0f, 1.0f);
	float  drag = 1;// 0.99;			// drag
	float damp = 0.5;
	bool   refl = false;
	
	float3 newPos = (float3)(pos[idx], pos[idx + 1], pos[idx + 2]);
	float3 force = (float3)(p_force[idx], p_force[idx + 1], p_force[idx + 2]);
	const float attract_str = 0.1;
	float2 d = normalize((float2)(attract_x - newPos.x, attract_y - newPos.y));
	float3 acc = 0.1f * force + (float3) (attract_str * d.x,
				           attract_str * d.y, 0);	// the strength of attraction to passed coords

	float3 newVel = (float3)(vel[idx], vel[idx + 1], vel[idx + 2]);

	float3 delta = (float3)(dt, dt, dt);

	newPos +=  delta * newVel;
	//newVel *= drag;
	newVel += delta * acc;
	
	if ( fabs ( newPos.x ) >= boxSize.x )
	{
		newPos = (float3)(pos[idx], pos[idx + 1], pos[idx + 2]);	// return to state before collision
		newVel.x = -newVel.x;
		refl     = true;
	}
	
	if ( fabs ( newPos.y ) >= boxSize.y )
	{
		newPos = (float3)(pos[idx], pos[idx + 1], pos[idx + 2]);	// return to state before collision
		newVel.y = -newVel.y;
		refl     = true;
	}
	
	if ( fabs ( newPos.z ) >= boxSize.z )
	{
		newPos = (float3)(pos[idx], pos[idx + 1], pos[idx + 2]);	// return to state before collision
		newVel.z = -newVel.z;
		refl     = true;
	}
	
	if ( refl )
		newVel *= damp;

	pos[idx  ] = newPos.x;
	pos[idx+1] = newPos.y;
	pos[idx+2] = newPos.z;

	vel[idx  ] = newVel.x;
	vel[idx+1] = newVel.y;
	vel[idx+2] = newVel.z;
}
