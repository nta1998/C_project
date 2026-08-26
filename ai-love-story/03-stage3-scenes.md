# שלב 3 — הסצנות

## מפת הסצנות

| סצנה | מה קורה | Elements | גנרציה |
|------|---------|----------|--------|
| **1 · הנמל** | 11 שוטים — מכניסת השחף ועד הקפיצה אחרי האונייה היוצאת | `@He_cruise`, `@She_cruise`, `@cruise` | 30s |
| **2a · הקרב** | קרב הענק — ההטלה, חטיפת החרב, ההפלה | `@He_arena`, `@giant`, `@arena`, `@sketch` | 25s |
| **2b · תא המלכה** | 8 קאטים — הדאגה המוסתרת שלה, היציאה המפלרטטת | `@He_arena`, `@She_arena`, `@giant`, `@arena`, `@throne`, `@sketch` | 25s |
| **3 · הרכבת** | שוד רכבת ווסטרן ב-16 שוטים, 3 סטוריבורדים נועלים את הלוגיקה המרחבית | `@He_train`, `@She_train`, `@train`, `@storyboard1..3` | 30s |
| **4a · העלייה לסיפון** | 9 קאטים — ממסירת המשקפת ועד הקרב שלה בדלתות המחסן | `@He_ship`, `@She_ship`, `@duelist_ship`, `@ship`, `@deck`, `@spyglass`, `@door`, `@crew` | 30s |
| **4b · הדו-קרב** | ביט קומי של 15 שניות — פוצל מ-4a כי הרצף לא נכנס בגנרציה אחת | `@He_ship`, `@duelist_ship`, `@deck`, `@door`, `@crew` | 15s |
| **5a · החיפוש** | פתיחת נשף המסכות, מסתיים במבט אחד מכוון בסלואו-מושן | `@He_carnival`, `@She_carnival`, `@carnival` | 15s |
| **5b · המצעד** | השניים מופרדים על ידי מצעד אינסופי; כרכרה חולפת ומעלימה אותו | `@He_carnival`, `@She_carnival`, `@carnival` | 15s |
| **5c · הנגיעה** | הפואנטה — החיפוש שלה, העצירה, נגיעת קצות האצבעות בסלואו-מושן | `@He_carnival`, `@She_carnival`, `@carnival` | 15s |
| **6 · המעלית** | פינאלה בימינו, עם מנגינה מזומזמת רוכבת על רפרנס אודיו מצורף | `@He_elevator`, `@She_elevator`, `@elevator`, `@headphones`, `@Audio` | 15s |

**סדר מומלץ לביצוע:** 1 → 5 → 6 → 4 → 2 → 3.
הנמל הוא המבחן (אם הפנים עוברות שם, הן יעברו בכל מקום), הקרנבל והמעלית הם הכי פשוטים
טכנית, והרכבת היא הכי מסובכת — תשאיר אותה לסוף.

---

# סצנה 1 — הנמל (הפרומפט המלא)

זה הפרומפט המרכזי, מותאם ל-tags שלכם. **החלף `@He_cruise` / `@She_cruise` בשמות
ה-Elements האמיתיים שלך** ותדביק ב-Cinema Studio.

**מודל:** Seedance 2.5 · **יחס:** 21:9 · **רזולוציה:** 1080p · **אורך:** 30s

```
11 shots, 22 seconds total, 2.39:1, anamorphic. Hard cuts — fast-cut rhythm. Slow motion ONLY inside SHOT 4 and inside SHOT 11 from the takeoff. One continuous story: a chance collision on a departure pier — a man falls in love in one look, lets her go, understands, and leaps.

SCENE ENGINE (production spine — the whole piece plays this chain)
GOAL (fixed, never changes): reach HER before the ship takes her — fix the mistake made one minute ago, when he let the encounter end.
OBSTACLE (global): the liner is leaving NOW — every second of the piece the goal is dying; (local): the packed pier, the crossing cart, the widening water gap.
TACTIC (search under incomplete knowledge): first he files the encounter away and walks off — a reasonable move for a man who doesn't yet know it was love; the failure of that tactic RETURNS INFORMATION — the sum won't balance, and the horn closes the truth gap. The threat then forces the only tactics left: the sprint, the vault, the leap.
REVERSAL (his own action flips on him): the walk he took to keep his dignity becomes the distance he must now sprint back; the man who let her go becomes the man leaping off a pier.
VALUE SHIFT (the audience's verdict moves): passer-by → polite romantic → sensible man who walks away → no — he's ALIVE, he's in love → madman → that's what a man is. Her verdict: stranger → cool beauty → no, she's interested.

REFERENCE DEFINITIONS
@cruise: working harbor pier, early 1900s — a long wide plank pier running to a great ocean liner at its end, dark hull, funnels pouring smoke into the hazy pale sky; bollards, nets, coiled ropes, crates and hand-carts along both edges; dock cranes and a forest of moored sailing-ship masts across the water, far hills; gulls in the haze. Geometry, materials and atmosphere only. Reference.
@He_cruise: young man — face, hair and wardrobe match the reference exactly in every frame: knee-length brown wool coat with notch lapels worn OPEN, dark-brown wool waistcoat, off-white band-collar shirt, brown wool trousers, worn brown leather lace-up boots, short dark side-parted hair, hands EMPTY, no hat. Reference.
@She_cruise: young woman — face, hair and wardrobe match the reference exactly in every frame: pale fabric hat with a dark ribbon that STAYS ON her head, long dark wavy hair, sage-grey long cape tied at the throat, cream high-collar lace blouse, grey ankle-length skirt, brown heeled ankle boots, a small pale canvas suitcase with leather corners in her LEFT hand — the LEFT hand in every frame she walks. Reference.

TECHNICAL BLOCK
Cinematic photoreal, ultra filmic early-1900s harbor pier. 21:9 — 35mm anamorphic film character: horizontal squeeze and compression, oval elliptical bokeh with stretched highlights, curved barrel edge distortion, subtle chromatic aberration toward the edges — all subtle and moderate; NO lens flares, NO streaks, NO floating bokeh circles. SOFT COLOR GRADE: gentle muted warm pastels, cream highlights, lifted soft blacks, low contrast — soft and faded. Shot on 35mm, fine period grain, hazy warm backlit daylight, sun soft behind the drifting funnel smoke — light identical first frame to last. Natural 180-degree shutter cadence — directional motion blur on fast moves, readable poses at holds; no ghosting, no duplicated limbs, no strobing. ALL HANDHELD, organic, never chaotic; SHOT 1 rides a smooth airborne glide, SHOT 11 a static level profile stage. SFX only.

LAWS:
- LOCATION LOCK: every pier shot — SHOTS 1–9 and SHOT 11 — is the SAME @cruise pier: plank geometry, bollards, crates, dock cranes, moored masts, far hills, funnel smoke and haze identical throughout; SHOT 10 is the deck of that same liner.
- FACE RULE: a lead's visible face fills at least a THIRD of frame height, chest-up or closer, fully inside the frame; at distance a lead reads from behind or in profile; eyes never look into the lens.
- LIVING FACE: the eyes always WORK at a task, never pose; a blink lands ONLY on a change of thought, slow and soft, a few per shot; never a frozen glassy stare. His eyes stay DRY throughout.
- FACE STABILITY: both faces photoreal, sharp, identity-locked every frame — no warping, in motion and slow motion alike.
- PROPS: HE carries nothing — both hands empty every frame; SHE carries exactly ONE small suitcase, only in her LEFT hand, low at her side.
- NO GLOW: no halo — sun draws only a thin realistic edge; shadow sides honest, never crushed.
- PORTRAIT DISTANCE: close framings come from a longer lens at distance — natural proportions, no wide-angle facial distortion.
- LEVEL CAMERA: horizon level, zero roll, verticals upright.
- LIVING CROWD (every frame on the pier): the pier is PACKED, each figure MID-ACTION every second — embraces, trunks, children darting, hand-carts, dockworkers, a fish crate trailed by gulls; density never drops, every body solid, nobody looks at the leads; passers-by wipe close past the lens every few seconds. In SHOTS 6–9 @She_cruise is never visible anywhere.
- SHIP & CHAIN (SHOT 11): the liner is UNDER WAY, sliding away past the pier's end, nothing connecting it to shore; the anchor chain hangs as ONE FREE VERTICAL LINE from the bow hawse straight down to the water, plumb, winched slowly up, links rising wet — oval links about thirty centimeters, forearm-thick, the chain narrower than his shoulders; no hooks, never diagonal, never touching the pier. The sea is fine irregular chop — flickering sparkle, wobbling reflections, no stripes, no patterns, never glassy.

SHOT 1 — 0.0–4.5s — ARRIVING WITH THE GULL — geography — aerial glide
First frame: open sky and haze — a single white GULL sweeps in beside the camera and the camera FLIES WITH IT, descending at 45 degrees over the packed artery of the @cruise pier: the passenger current flowing toward the glowing liner, crates heaved along the edges, a hand-cart cutting a seam, children darting, long soft shadows. The gull banks away over the crowd as the shot ends. No lead identifiable.

SHOT 2 — 4.5–5.5s — HIS BOOTS / HER BOOTS
Hard cut. Ankle height, back-tracking: @He_cruise's boots stride FAST at the camera, firm heel-to-toe, dust puffing — the only boots against the current. Mid-shot hard cut: @She_cruise's boots hurry at the camera WITH the flow — quick light steps, hem swaying, the pale suitcase corner swinging at her LEFT side.

SHOT 3 — 5.5–6.5s — THE HIT
Hard cut, shoulder height, close. STAGING: @He_cruise enters frame-RIGHT (x≈70%) moving screen-left, chest and face toward HER; @She_cruise enters frame-LEFT (x≈30%) moving screen-right, chest and face toward HIM, suitcase in her LEFT hand — both within one meter of the lens, chests facing each other, never backs. Within half a second their right shoulders CLIP beside the camera: the suitcase swings wide once, her cape flares, his torso twists a quarter-turn, one faltered half-step each, a sharp intake of breath. Momentum carries each ONE step past the other.

SHOT 4 — 6.5–9.5s — THE FULL TURN — HE FALLS IN LOVE — slow motion
Hard cut, whole shot in smooth slow motion, chest height. STAGING: the pass has put @He_cruise frame-LEFT (x≈35%) and @She_cruise frame-RIGHT (x≈65%), one meter of hazy air between them; BOTH stop and pivot FULLY toward each other on planted feet until they stand squared face to face, the crowd parting around them like water around stones.
ACTING TASK — HIM (the work happens in his eyes): task — understand what just happened to him; he checks both her eyes, one then the other, reading whether she noticed him noticing — and mid-look the guard drops, the eyes soften and widen a fraction, the breath forgets itself: he is FALLING IN LOVE right here and doesn't know the word for it yet. New, helpless, disbelieving.
ACTING TASK — HER: task — measure the stranger one beat past polite and decide he's worth a smile; she holds his eyes, then a real soft smile blooms — given freely, no promise attached. His answering smile arrives one beat AFTER hers. One soft slowed blink from her across the whole shot.
Golden backlit dust hangs; strands of her hair float across her cheek in the rim light; her cape settles in floating folds.

SHOT 5 — 9.5–10.5s — SHE GOES WHERE SHE WAS GOING
Hard cut, real speed crashes back. Camera just behind @He_cruise's right shoulder, his jaw soft in the near-right foreground: @She_cruise breaks the look FIRST — turns and hurries on the way she was always going, toward the ship, back to camera, suitcase in her LEFT hand, weaving through gaps; passers-by wipe the near foreground and after each wipe she is farther, her hat sinking between shoulders. She does not look back. He stays planted, turned after her.

SHOT 6 — 10.5–13.5s — HE WATCHES HER GO — the turn — his back to the ship — the heavy walk
Hard cut: close slight three-quarter on @He_cruise from ahead-right, 47mm, the living crowd soft at every depth.
ACTING TASK — HIM: scene direction (the whole pier lives it): partings are the way of things — you let people go and get on with your day. His motive: a grown man doesn't chase a stranger's ship. His goal: file the encounter away and walk off clean. His tactic — ESCORT her with his eyes as long as the crowd allows (smooth pursuit, a fraction of head drift), hold the empty spot half a breath when the crowd takes her, one slow blink exactly on the loss — then the head DIPS, he TURNS and walks with his BACK to the liner, slowly toward the camera, eyes down, working the planks like a man re-checking an arithmetic that won't come out even; each step re-closes the drawer and each step it swings back open; his pace loses a fraction per step. The camera back-tracks glued to the framing.

SHOT 7 — 13.5–15.5s — THE HORN — HE REALIZES — THE PIVOT
Same setup, tighter: @He_cruise's face large in three-quarter, the liner far behind his back. A LONG DEEP HORN rolls over the pier — and the truth gap CLOSES: he has fallen in love, and he is LOSING her, right now, to that ship. His step DIES mid-stride; the eyes SNAP up off the planks and go still — breath catches and shortens, jaw sets, one knocked-out blink; a half-turn of the head back toward the ship — the eyes sharpen, decided — he WHIPS around over his left shoulder, head snapping first with real motion blur, body following on a planted boot, coat flaring — and explodes into the sprint after her. The face never performs — the task does everything.

SHOT 8 — 15.5–16.5s — THE CART VAULT — head-on
Hard cut. LOW back-tracking in front of @He_cruise, knee height, real speed — a loaded HAND-CART cuts across from frame-left, its puller leaning into the shafts — he does NOT slow: two loading strides, he LEAPS, one palm tapping the top crate, boots swinging clean OVER the load toward the camera, coat flying, dust bursting as he LANDS without breaking stride — the cart-man's head snaps after him.

SHOT 9 — 16.5–17.5s — THE RUN — close three-quarter
Hard cut. Camera back-tracks fast ahead-right of him, 47mm: @He_cruise's face LARGE in three-quarter, breath tearing, arms driving, jaw carved by the backlight; the crowd blurs behind his far shoulder, heads turning after him.
ACTING TASK: his eyes CATCH THE SHIP beyond the camera and hold it like a rope — measuring the shrinking distance stride by stride, recalculating the path through the crowd without letting go.

SHOT 10 — 17.5–18.5s — HER PROFILE AT THE RAIL — lightly nonchalant
Hard cut — high on the ship: @She_cruise's clean PROFILE close-up at the deck rail against the open hazy golden sky, hat line sharp against the light, sea wind lifting her hair back, one hand resting easy on the worn rail.
ACTING TASK — HER: task — appraise the running stranger below without giving anything away; her eyes angle DOWN to the pier and snag on the running figure: a soft huff of an exhale through the nose, the visible corner of her mouth curling into a faint warm half-smile, one slow amused narrowing of the eye — cool on the surface, a spark of real interest underneath — and she keeps watching, the little smile holding in profile. One soft blink on the moment she notices. No big emotion.

SHOT 11 — 18.5–22.0s — THE FINAL SPRINT AND THE LEAP — static profile
Hard cut. One breath of the sprint from behind-side: the last bare planks of the @cruise pier rushing under @He_cruise's boots, the towering dark bow sliding AWAY past the pier's end, the wet anchor chain climbing link by link out of the choppy water — he BURSTS, head dropping into full commitment — then the STATIC profile stage, level, perpendicular, 1.5 meters above the water. STAGING: the pier edge lower-LEFT with its empty planks; the vertical chain at x≈65% falling from the top edge to the wet anchor at the water (y≈75%); the dark hull edge slicing the extreme frame-RIGHT, gliding away; cranes and moored ships far in the haze; his flight crosses the CENTER at the upper third, left to right. His last stride PLANTS on the edge plank — it flexes, dust kicks — and EXACTLY on the takeoff the shot ramps into smooth slow motion: one short committed spring over the choppy gap, body stretching fully HORIZONTAL like a swimmer, coat flaring behind, one white gull gliding above the arc — both arms driving forward, hands OPEN, fingers spreading toward the wet links just above the anchor — closer — centimeters. Final frame HELD: full wide profile, mid-air at the top of the reach, hands a palm's width from the rising iron, spray suspended, the gull mid-wingbeat. This exact frame is the last frame.

PHYSICS
Both leads hurry with real weight; the collision has true mass — torsos rotate, one faltered half-step each, never backward; the full turns pivot on planted feet. His heavy walk carries honest weight; the dead stop lands on a planted heel; the whip pivot turns head-first with motion blur; the sprint explodes through real acceleration and holds full tilt. The vault is true parkour physics — the palm takes real weight, hips swing clean, the landing leg rolls back into stride, the cart rocks, nothing tips. The leap is true ballistics — one driving-leg takeoff, a shallow rising arc, no floating, no mid-air correction; slow motion stretches time, never the trajectory. The chain hangs plumb under its own weight, rising at a steady winch pace, shedding scattered heavy drops. Cloth, hair and the suitcase carry natural delay, exaggerated only in slow motion; every figure solid; the gull flies with real wing mechanics.

SFX only: from the air — wind, the gull's wingbeats and one cry, the dock's wash rising with the descent — dropping INTO the crowd: layered boots, urgent murmur, crate thuds, cart wheels, gulls; his firm strikes and her quick light steps; the soft cloth THUD of the hit, a sharp intake of breath — the world dipping muffled through the slow-motion turn, one slowed gull cry, a single close breath of each — crashing back as she leaves; farewells washing under his held breath, his slowing steps — the LONG DEEP HORN over everything, one caught breath in the near-silence, the whip of cloth on the pivot — the rapid hammer of the sprint, grinding cart wheels, the slap of his palm, the double thud of the vault — one beat of high sea wind and her amused exhale at the rail — boots on the last planks, the plank's creak under the plant, the deep wash of the moving hull, the rhythmic clank of the windlass — and the world falls away with the leap into a soft airy hush: wind, heartbeat, one slowed gull cry, water flicking off the rising links — the piece ends inside the held breath of the reach. No dialogue, no music. 35mm grain, soft faded period-drama realism.
```

---

## הסצנות הבאות

הפרומפטים לסצנות 2–6 ייכתבו כשנגיע אליהן, אחרי שנראה איך הפנים שלכם מתנהגות
בסצנת הנמל. תגיד לי איזו סצנה — אני מריץ את `higgsfield-seedance-prompt-builder`
ומחזיר פרומפט מלא באותו מבנה.

**כלל הזהב לכל סצנה חדשה:** להעתיק את ה-TECHNICAL BLOCK מסצנה 1 מילה במילה.
אותה אופטיקה, אותו גרעין, אותו גרייד — זה מה שהופך שש תקופות לסרט אחד.
