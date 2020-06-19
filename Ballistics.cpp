#include "ShooterGame.h"
#include "Ballistics.h"

#define BC_CONVERSION_FACTOR 0.00068418f
#define SPEED_OF_SOUND 343.0f

void UBallisticsSubSystem::Initialize(FSubsystemCollectionBase& Collection) {
	ready = true;
}

void UBallisticsSubSystem::Deinitialize() {
	Clear();
}

bool UBallisticsSubSystem::IsTickable() const {
	return ready;
}

TStatId UBallisticsSubSystem::GetStatId() const {
	return TStatId();
}

void UBallisticsSubSystem::Clear() {
	Projectiles.Empty();
}

float UBallisticsSubSystem::CalculateDeceleration(int DragFunction, float BallisticCoefficient, float Velocity) {
	// Convert cm/s to m/s
	Velocity /= 100;

	// Drag Functions from: http://www.jbmballistics.com/ballistics/downloads/text/
	const TArray<TArray<float>> M = {
		{ 0.00f, 0.05f, 0.10f, 0.15f, 0.20f, 0.25f, 0.30f, 0.35f, 0.40f, 0.45f, 0.50f, 0.55f, 0.60f, 0.70f, 0.725f, 0.75f, 0.775f, 0.80f, 0.825f, 0.85f, 0.875f, 0.90f, 0.925f, 0.95f, 0.975f, 1.0f, 1.025f, 1.05f, 1.075f, 1.10f, 1.125f, 1.15f, 1.20f, 1.25f, 1.30f, 1.35f, 1.40f, 1.45f, 1.50f, 1.55f, 1.60f, 1.65f, 1.70f, 1.75f, 1.80f, 1.85f, 1.90f, 1.95f, 2.00f, 2.05f, 2.10f, 2.15f, 2.20f, 2.25f, 2.30f, 2.35f, 2.40f, 2.45f, 2.50f, 2.60f, 2.70f, 2.80f, 2.90f, 3.00f, 3.10f, 3.20f, 3.30f, 3.40f, 3.50f, 3.60f, 3.70f, 3.80f, 3.90f, 4.00f, 4.20f, 4.40f, 4.60f, 4.80f, 5.00f },
		{ 0.00f, 0.05f, 0.10f, 0.15f, 0.20f, 0.25f, 0.30f, 0.35f, 0.40f, 0.45f, 0.50f, 0.55f, 0.60f, 0.65f, 0.70f, 0.75f, 0.775f, 0.80f, 0.825f, 0.85f, 0.875f, 0.90f, 0.925f, 0.95f, 0.975f, 1.0f, 1.025f, 1.05f, 1.075f, 1.10f, 1.125f, 1.15f, 1.175f, 1.20f, 1.25f, 1.30f, 1.35f, 1.40f, 1.45f, 1.50f, 1.55f, 1.60f, 1.65f, 1.70f, 1.75f, 1.80f, 1.85f, 1.90f, 1.95f, 2.00f, 2.05f, 2.10f, 2.15f, 2.20f, 2.25f, 2.30f, 2.35f, 2.40f, 2.45f, 2.50f, 2.55f, 2.60f, 2.65f, 2.70f, 2.75f, 2.80f, 2.85f, 2.90f, 2.95f, 3.00f, 3.10f, 3.20f, 3.30f, 3.40f, 3.50f, 3.60f, 3.70f, 3.80f, 3.90f, 4.00f, 4.20f, 4.40f, 4.60f, 4.80f, 5.00f },
		{ 0.00f, 0.05f, 0.10f, 0.15f, 0.20f, 0.25f, 0.30f, 0.35f, 0.40f, 0.45f, 0.50f, 0.55f, 0.60f, 0.65f, 0.70f, 0.75f, 0.80f, 0.85f, 0.875f, 0.90f, 0.925f, 0.95f, 0.975f, 1.0f, 1.025f, 1.05f, 1.075f, 1.10f, 1.15f, 1.20f, 1.25f, 1.30f, 1.35f, 1.40f, 1.45f, 1.50f, 1.55f, 1.60f, 1.65f, 1.70f, 1.75f, 1.80f, 1.85f, 1.90f, 1.95f, 2.00f, 2.05f, 2.10f, 2.15f, 2.20f, 2.25f, 2.30f, 2.35f, 2.40f, 2.45f, 2.50f, 2.60f, 2.70f, 2.80f, 2.90f, 3.00f, 3.10f, 3.20f, 3.30f, 3.40f, 3.50f, 3.60f, 3.70f, 3.80f, 3.90f, 4.00f, 4.20f, 4.40f, 4.60f, 4.80f, 5.00 },
		{ 0.00f, 0.05f, 0.10f, 0.15f, 0.20f, 0.25f, 0.30f, 0.35f, 0.40f, 0.45f, 0.50f, 0.55f, 0.60f, 0.65f, 0.70f, 0.75f, 0.80f, 0.85f, 0.875f, 0.90f, 0.925f, 0.95f, 0.975f, 1.0f, 1.025f, 1.05f, 1.075f, 1.10f, 1.125f, 1.15f, 1.175f, 1.20f, 1.225f, 1.25f, 1.30f, 1.35f, 1.40f, 1.45f, 1.50f, 1.55f, 1.60f, 1.65f, 1.70f, 1.75f, 1.80f, 1.85f, 1.90f, 1.95f, 2.00f, 2.05f, 2.10f, 2.15f, 2.20f, 2.25f, 2.30f, 2.35f, 2.40f, 2.45f, 2.50f, 2.60f, 2.70f, 2.80f, 2.90f, 3.00f, 3.10f, 3.20f, 3.30f, 3.40f, 3.50f, 3.60f, 3.70f, 3.80f, 3.90f, 4.00f, 4.20f, 4.40f, 4.60f, 4.80f, 5.00 },
		{ 0.00f, 0.05f, 0.10f, 0.15f, 0.20f, 0.25f, 0.30f, 0.35f, 0.40f, 0.45f, 0.50f, 0.55f, 0.60f, 0.65f, 0.70f, 0.725f, 0.75f, 0.775f, 0.80f, 0.825f, 0.85f, 0.875f, 0.90f, 0.925f, 0.95f, 0.975f, 1.0f, 1.025f, 1.05f, 1.075f, 1.10f, 1.125f, 1.15f, 1.20f, 1.25f, 1.30f, 1.35f, 1.40f, 1.50f, 1.55f, 1.60f, 1.65f, 1.70f, 1.75f, 1.80f, 1.85f, 1.90f, 1.95f, 2.00f, 2.05f, 2.10f, 2.15f, 2.20f, 2.25f, 2.30f, 2.35f, 2.40f, 2.45f, 2.50f, 2.55f, 2.60f, 2.65f, 2.70f, 2.75f, 2.80f, 2.85f, 2.90f, 2.95f, 3.00f, 3.10f, 3.20f, 3.30f, 3.40f, 3.50f, 3.60f, 3.70f, 3.80f, 3.90f, 4.00f, 4.20f, 4.40f, 4.60f, 4.80f, 5.00 },
		{ 0.00f, 0.05f, 0.10f, 0.15f, 0.20f, 0.25f, 0.30f, 0.35f, 0.40f, 0.45f, 0.50f, 0.55f, 0.60f, 0.65f, 0.70f, 0.75f, 0.80f, 0.825f, 0.85f, 0.875f, 0.90f, 0.925f, 0.95f, 0.975f, 1.0f, 1.025f, 1.05f, 1.075f, 1.10f, 1.125f, 1.15f, 1.20f, 1.25f, 1.30f, 1.35f, 1.40f, 1.45f, 1.50f, 1.55f, 1.60f, 1.65f, 1.70f, 1.75f, 1.80f, 1.85f, 1.90f, 1.95f, 2.00f, 2.05f, 2.10f, 2.15f, 2.20f, 2.25f, 2.30f, 2.35f, 2.40f, 2.45f, 2.50f, 2.60f, 2.70f, 2.80f, 2.90f, 3.00f, 3.10f, 3.20f, 3.30f, 3.40f, 3.50f, 3.60f, 3.70f, 3.80f, 3.90f, 4.00f, 4.20f, 4.40f, 4.60f, 4.80f, 5.00f }
	};

	const TArray<TArray<float>> CD = {
		{ 0.2629f, 0.2558f, 0.2487f, 0.2413f, 0.2344f, 0.2278f, 0.2214f, 0.2155f, 0.2104f, 0.2061f, 0.2032f, 0.2020f, 0.2034f, 0.2165f, 0.2230f, 0.2313f, 0.2417f, 0.2546f, 0.2706f, 0.2901f, 0.3136f, 0.3415f, 0.3734f, 0.4084f, 0.4448f, 0.4805f, 0.5136f, 0.5427f, 0.5677f, 0.5883f, 0.6053f, 0.6191f, 0.6393f, 0.6518f, 0.6589f, 0.6621f, 0.6625f, 0.6607f, 0.6573f, 0.6528f, 0.6474f, 0.6413f, 0.6347f, 0.6280f, 0.6210f, 0.6141f, 0.6072f, 0.6003f, 0.5934f, 0.5867f, 0.5804f, 0.5743f, 0.5685f, 0.5630f, 0.5577f, 0.5527f, 0.5481f, 0.5438f, 0.5397f, 0.5325f, 0.5264f, 0.5211f, 0.5168f, 0.5133f, 0.5105f, 0.5084f, 0.5067f, 0.5054f, 0.5040f, 0.5030f, 0.5022f, 0.5016f, 0.5010f, 0.5006f, 0.4998f, 0.4995f, 0.4992f, 0.4990f, 0.4988f },
		{ 0.2303f, 0.2298f, 0.2287f, 0.2271f, 0.2251f, 0.2227f, 0.2196f, 0.2156f, 0.2107f, 0.2048f, 0.1980f, 0.1905f, 0.1828f, 0.1758f, 0.1702f, 0.1669f, 0.1664f, 0.1667f, 0.1682f, 0.1711f, 0.1761f, 0.1831f, 0.2004f, 0.2589f, 0.3492f, 0.3983f, 0.4075f, 0.4103f, 0.4114f, 0.4106f, 0.4089f, 0.4068f, 0.4046f, 0.4021f, 0.3966f, 0.3904f, 0.3835f, 0.3759f, 0.3678f, 0.3594f, 0.3512f, 0.3432f, 0.3356f, 0.3282f, 0.3213f, 0.3149f, 0.3089f, 0.3033f, 0.2982f, 0.2933f, 0.2889f, 0.2846f, 0.2806f, 0.2768f, 0.2731f, 0.2696f, 0.2663f, 0.2632f, 0.2602f, 0.2572f, 0.2543f, 0.2515f, 0.2487f, 0.2460f, 0.2433f, 0.2408f, 0.2382f, 0.2357f, 0.2333f, 0.2309f, 0.2262f, 0.2217f, 0.2173f, 0.2132f, 0.2091f, 0.2052f, 0.2014f, 0.1978f, 0.1944f, 0.1912f, 0.1851f, 0.1794f, 0.1741f, 0.1693f, 0.1648f },
		{ 0.1710f, 0.1719f, 0.1727f, 0.1732f, 0.1734f, 0.1730f, 0.1718f, 0.1696f, 0.1668f, 0.1637f, 0.1603f, 0.1566f, 0.1529f, 0.1497f, 0.1473f, 0.1463f, 0.1489f, 0.1583f, 0.1672f, 0.1815f, 0.2051f, 0.2413f, 0.2884f, 0.3379f, 0.3785f, 0.4032f, 0.4147f, 0.4201f, 0.4278f, 0.4338f, 0.4373f, 0.4392f, 0.4403f, 0.4406f, 0.4401f, 0.4386f, 0.4362f, 0.4328f, 0.4286f, 0.4237f, 0.4182f, 0.4121f, 0.4057f, 0.3991f, 0.3926f, 0.3861f, 0.3800f, 0.3741f, 0.3684f, 0.3630f, 0.3578f, 0.3529f, 0.3481f, 0.3435f, 0.3391f, 0.3349f, 0.3269f, 0.3194f, 0.3125f, 0.3060f, 0.2999f, 0.2942f, 0.2889f, 0.2838f, 0.2790f, 0.2745f, 0.2703f, 0.2662f, 0.2624f, 0.2588f, 0.2553f, 0.2488f, 0.2429f, 0.2376f, 0.2326f, 0.2280f },
		{ 0.2617f, 0.2553f, 0.2491f, 0.2432f, 0.2376f, 0.2324f, 0.2278f, 0.2238f, 0.2205f, 0.2177f, 0.2155f, 0.2138f, 0.2126f, 0.2121f, 0.2122f, 0.2132f, 0.2154f, 0.2194f, 0.2229f, 0.2297f, 0.2449f, 0.2732f, 0.3141f, 0.3597f, 0.3994f, 0.4261f, 0.4402f, 0.4465f, 0.4490f, 0.4497f, 0.4494f, 0.4482f, 0.4464f, 0.4441f, 0.4390f, 0.4336f, 0.4279f, 0.4221f, 0.4162f, 0.4102f, 0.4042f, 0.3981f, 0.3919f, 0.3855f, 0.3788f, 0.3721f, 0.3652f, 0.3583f, 0.3515f, 0.3447f, 0.3381f, 0.3314f, 0.3249f, 0.3185f, 0.3122f, 0.3060f, 0.3000f, 0.2941f, 0.2883f, 0.2772f, 0.2668f, 0.2574f, 0.2487f, 0.2407f, 0.2333f, 0.2265f, 0.2202f, 0.2144f, 0.2089f, 0.2039f, 0.1991f, 0.1947f, 0.1905f, 0.1866f, 0.1794f, 0.1730f, 0.1673f, 0.1621f, 0.1574f },
		{ 0.1198f, 0.1197f, 0.1196f, 0.1194f, 0.1193f, 0.1194f, 0.1194f, 0.1194f, 0.1193f, 0.1193f, 0.1194f, 0.1193f, 0.1194f, 0.1197f, 0.1202f, 0.1207f, 0.1215f, 0.1226f, 0.1242f, 0.1266f, 0.1306f, 0.1368f, 0.1464f, 0.1660f, 0.2054f, 0.2993f, 0.3803f, 0.4015f, 0.4043f, 0.4034f, 0.4014f, 0.3987f, 0.3955f, 0.3884f, 0.3810f, 0.3732f, 0.3657f, 0.3580f, 0.3440f, 0.3376f, 0.3315f, 0.3260f, 0.3209f, 0.3160f, 0.3117f, 0.3078f, 0.3042f, 0.3010f, 0.2980f, 0.2951f, 0.2922f, 0.2892f, 0.2864f, 0.2835f, 0.2807f, 0.2779f, 0.2752f, 0.2725f, 0.2697f, 0.2670f, 0.2643f, 0.2615f, 0.2588f, 0.2561f, 0.2533f, 0.2506f, 0.2479f, 0.2451f, 0.2424f, 0.2368f, 0.2313f, 0.2258f, 0.2205f, 0.2154f, 0.2106f, 0.2060f, 0.2017f, 0.1975f, 0.1935f, 0.1861f, 0.1793f, 0.1730f, 0.1672f, 0.1618f },
		{ 0.2105f, 0.2105f, 0.2104f, 0.2104f, 0.2103f, 0.2103f, 0.2103f, 0.2103f, 0.2103f, 0.2102f, 0.2102f, 0.2102f, 0.2102f, 0.2102f, 0.2103f, 0.2103f, 0.2104f, 0.2104f, 0.2105f, 0.2106f, 0.2109f, 0.2183f, 0.2571f, 0.3358f, 0.4068f, 0.4378f, 0.4476f, 0.4493f, 0.4477f, 0.4450f, 0.4419f, 0.4353f, 0.4283f, 0.4208f, 0.4133f, 0.4059f, 0.3986f, 0.3915f, 0.3845f, 0.3777f, 0.3710f, 0.3645f, 0.3581f, 0.3519f, 0.3458f, 0.3400f, 0.3343f, 0.3288f, 0.3234f, 0.3182f, 0.3131f, 0.3081f, 0.3032f, 0.2983f, 0.2937f, 0.2891f, 0.2845f, 0.2802f, 0.2720f, 0.2642f, 0.2569f, 0.2499f, 0.2432f, 0.2368f, 0.2308f, 0.2251f, 0.2197f, 0.2147f, 0.2101f, 0.2058f, 0.2019f, 0.1983f, 0.1950f, 0.1890f, 0.1837f, 0.1791f, 0.1750f, 0.1713f }
	};

	float m = Velocity / SPEED_OF_SOUND;

	// Mach must be lower than 5
	check(m < 5.0)

		for (int i = 0; i < M[DragFunction].Num(); i++) {
			if (M[DragFunction][i] >= m) {
				int pI = FPlatformMath::Max(0, i - 1);
				float pCD = CD[DragFunction][pI];
				float pM = M[DragFunction][pI];

				// Interpolate between the two
				float cd = pCD + (CD[DragFunction][i] - pCD) * (m - pM) / (M[DragFunction][i] - pM);

				// return cm/s
				return (BC_CONVERSION_FACTOR * (cd / BallisticCoefficient) * FMath::Pow(Velocity, 2)) * 100;
			}
		}

	return 0.0;
}

FCollisionQueryParams FBallisticInFlight::TraceParameters()
{
	FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(WeaponTrace));
	TraceParams.bReturnPhysicalMaterial = true;
	TraceParams.bReturnFaceIndex = true;
	/*TraceParams.bFindInitialOverlaps = true;
	TraceParams.bIgnoreTouches = true;*/
	TraceParams.bTraceComplex = true;
	TraceParams.bDebugQuery = false;
	return TraceParams;
}

// Find an exit point for an impact
FVector FBallisticInFlight::CheckForExit(UWorld *World, FVector ImpactPoint, FVector Direction, TWeakObjectPtr<AActor> Ignore) {
	FCollisionQueryParams TraceParams = TraceParameters();

	FVector startPosition = ImpactPoint;
	FVector endPosition = ImpactPoint + (Direction * 50.f);

	// Ignore the actor we hit to prevent tracing through.
	if (Ignore.IsValid())
		TraceParams.AddIgnoredActor(Ignore.Get());

	// Trace forward for any future hit.
	TArray<FHitResult> LookForward;

	// First hits can be in penetration so we only care about the last hit if it didn't start inside an object.
	bool HasHit = World->LineTraceMultiByChannel(LookForward, startPosition, endPosition, COLLISION_WEAPON, TraceParams);
	DrawDebugCircle(World, ImpactPoint, 2.0f, 1, FColor(0, 200, 200), true);
	if (HasHit && LookForward.Last().bStartPenetrating == false) {
		DrawDebugLine(World, startPosition, LookForward.Last().ImpactPoint, FColor(0, 200, 200), true, 2.0f);
		endPosition = LookForward.Last().ImpactPoint;
	}

	// Remove any ignored actors to find exits on any that started in pen.
	TraceParams.ClearIgnoredActors();

	// Trace backwards for an exit to the impact
	TArray<FHitResult> LookBackwards;
	 HasHit = World->LineTraceMultiByChannel(LookBackwards, endPosition, ImpactPoint, COLLISION_WEAPON, TraceParams);
	if (HasHit) {
		DrawDebugLine(World, endPosition, LookBackwards.Last().ImpactPoint, FColor(200, 100, 0), true, 2.0f, 0, 2.0f);
		DrawDebugCircle(World, LookBackwards.Last().ImpactPoint, 3.0f, 1, FColor(200, 100, 0), true);
		return LookBackwards.Last().ImpactPoint + (Direction * 0.5f); // 50mm forward to prevent re-entry on next trace
	}

	// No exit, died in penetration.
	return FVector::ZeroVector;
}

void UBallisticsSubSystem::Tick(float DeltaTime) {
	if (Projectiles.Num() > 0) {
		// Tick each projectile
		for (auto& e : Projectiles) {
			for (int i = e.Value.Num() - 1; i >= 0; i--) {
				auto& p = e.Value[i];
				bool atEnd = false;
				bool finish = false;

				// G tables are in m/s and I am too lazy to convert everything to cm/s
				float acceleration = CalculateDeceleration(0, p.BallisticCoefficient, p.Velocity.Size()) * DeltaTime;
				float accelerationfactor = (p.Velocity.Size() - acceleration) / p.Velocity.Size();

				// Add gravity and drag
				FVector newPosition = p.Position + (p.Velocity * DeltaTime);
				FVector startPosition = p.Position;
				DrawDebugLine(GetWorld(), startPosition, newPosition, FColor(0, 0, 0), true, 0.1f);

				p.Velocity *= accelerationfactor;
				p.Velocity.Z += GetWorld()->GetGravityZ() * DeltaTime;

				FCollisionQueryParams TraceParams = p.TraceParameters();

				while (!atEnd)
				{
					// Perform trace to retrieve hit info
					FHitResult Hit;
					bool HasHit = GetWorld()->LineTraceSingleByChannel(Hit, startPosition, newPosition, COLLISION_WEAPON, TraceParams);

					if (HasHit) {
						// Complete any exits prior to this hit
						startPosition = p.CheckForExit(GetWorld(), Hit.ImpactPoint, p.Vector, Hit.Actor);
						if (startPosition == FVector::ZeroVector) {
							atEnd = true;
							finish = true;
						}

						// Exit if we go past penetration check
						if (FVector::Distance(startPosition, newPosition) <= 50.0f)
							atEnd = true;
					}
					else {
						atEnd = true;
					}
				}

				p.Position = newPosition;

				// Remove any beyond lifetime or stupid low velocity.
				p.Lifetime -= DeltaTime;
				if (p.Lifetime <= 0.0f || finish || p.Velocity.Size() < 1000) {
					e.Value.RemoveAt(i);
				}
			}
		}
	}
}

void UBallisticsSubSystem::AddProjectile(APawn* character, FVector Position, FVector Direction) {
	Projectiles.FindOrAdd(character)
		.Add(FBallisticInFlight{
				Position,
				Direction,
				Direction * 87782.4f,
				0.547f,
				15
			});
}


