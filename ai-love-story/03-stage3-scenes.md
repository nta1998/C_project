# שלב 3 — הסצנות

## מפת הסצנות

| סצנה | מה קורה | Elements | גנרציה |
|------|---------|----------|--------|
| **1 · הנמל** | 10 שוטים, 18 שניות — מכניסת השחף ועד הקפיצה אחרי האונייה היוצאת | `@He_cruise`, `@She_cruise`, `@cruise` | 18s |
| **2 · הזירה** | 11 שוטים, 18 שניות — הקרב שלו והתא שלה, משולבים | `@He_arena`, `@She_arena`, `@robot`, `@arena`, `@sketch` | 18s |
| **3 · הרכבת** | 10 שוטים, 18 שניות — הוא עולה על רכבת נוסעת, היא צוללת מגשר | `@He_train`, `@She_train`, `@train`, `@boards` | 18s |
| ~~4 · הספינה~~ | הוחלף במטבח | — | — |
| **7 · המטבח** | 6 שוטים, 15 שניות — ערב רגיל, והוא מעביר לה קערת זיתים | `@He_kitchen`, `@She_kitchen`, `@kitchen`, `@bowl` | 15s |
| **5 · הקרנבל** | 9 שוטים, 18 שניות — היציאה מהארמון, החיפוש, המבט האחד, נגיעת קצות האצבעות | `@He_carnival`, `@She_carnival`, `@carnival` | 18s |
| **6 · המעלית** | פינאלה בימינו, עם מנגינה מזומזמת רוכבת על רפרנס אודיו מצורף | `@He_elevator`, `@She_elevator`, `@elevator`, `@headphones`, `@Audio` | 15s |

**סדר מומלץ לביצוע:** 1 → 5 → 6 → 4 → 2 → 3.
הנמל הוא המבחן (אם הפנים עוברות שם, הן יעברו בכל מקום), הקרנבל והמעלית הם הכי פשוטים
טכנית, והרכבת היא הכי מסובכת — תשאיר אותה לסוף.

---

# סצנה 1 — הנמל (הפרומפט המלא)

זה הפרומפט המרכזי, מותאם ל-tags שלכם. **החלף `@He_cruise` / `@She_cruise` בשמות
ה-Elements האמיתיים שלך** ותדביק ב-Cinema Studio.

**מודל:** Seedance 2.5 · **יחס:** 21:9 · **רזולוציה:** 1080p · **אורך גנרציה:** הקצר ביותר שזמין מעל 18 שניות

```
10 shots, 18 seconds total, 2.39:1, anamorphic. Hard cuts — fast-cut rhythm. Slow motion ONLY inside SHOT 3 and inside SHOT 10 from the takeoff. One continuous story: a chance collision on a departure pier — a man falls in love in one look, lets her go, understands, and leaps.

SCENE ENGINE (production spine — the whole piece plays this chain)
GOAL (fixed, never changes): reach HER before the ship takes her — fix the mistake made one minute ago, when he let the encounter end.
OBSTACLE (global): the liner is leaving NOW — every second of the piece the goal is dying; (local): the packed pier, the crossing cart, the widening water gap.
TACTIC (search under incomplete knowledge): first he files the encounter away and walks off — a reasonable move for a man who doesn't yet know it was love; the failure of that tactic RETURNS INFORMATION — the sum won't balance, and the horn closes the truth gap. The threat then forces the only tactics left: the sprint, the vault, the leap.
REVERSAL (his own action flips on him): the walk he took to keep his dignity becomes the distance he must now sprint back; the man who let her go becomes the man leaping off a pier.
VALUE SHIFT (the audience's verdict moves): passer-by → polite romantic → sensible man who walks away → no — he's ALIVE, he's in love → madman → that's what a man is. Her verdict: stranger → cool beauty → no, she's interested.

REFERENCE DEFINITIONS
@cruise: working harbor pier, early 1900s — a long wide worn timber plank pier running into the depth to a great ocean liner moored at its far end: black hull with a red boot-topping, white superstructure, tall black-and-white banded funnels pouring dark coal smoke into a pale hazy sky, a heavy anchor chain hanging from the bow hawse to the water, a wooden boarding gangway with rope-and-iron railings rising from the pier to the hull. Stacked wooden crates, barrels, sacks, coiled rope and cargo nets along the LEFT edge with a stone beacon tower beyond them; tall timber A-frame loading cranes with hanging chains and hooks along the RIGHT edge. Telegraph lines strung across the sky overhead; a forest of moored tall-masted sailing ships dissolving into sea haze on both sides; open hazy sea horizon beyond, no hills; gulls in the haze. Geometry, materials and atmosphere only. Reference.
@He_cruise: young man — face, hair and wardrobe match the reference exactly in every frame: dark charcoal-brown patterned wool sack coat with notch lapels worn OPEN, gathered at the shoulder seams; a matching patterned waistcoat buttoned over an off-white shirt with a narrow dark tie; matching patterned wool trousers; worn dark brown leather lace-up boots. Short thick dark wavy hair swept up and tousled with a loose strand falling on the forehead; short dark beard and moustache. Hands EMPTY, no hat. Reference.
@She_cruise: young woman — face, hair and wardrobe match the reference exactly in every frame: a wide-brimmed pale grey-beige woven traveling hat with a slate-grey ribbon band and small muted feathers at the side, worn level and STAYING ON her head; long thick dark-brown wavy hair loose well past her shoulders; light blue eyes, strong dark brows; a long light greige wool traveling cape with a layered shoulder capelet, tied at the throat with a thin ribbon, ending at mid-calf and hanging clear of the ground; a cream high-collar blouse with a ruffled lace placket; a long charcoal-grey wool skirt to the ankle; brown leather lace-up low-heel boots; a small pale canvas suitcase with brown leather trim and corner caps in her RIGHT hand — the RIGHT hand in every frame she walks. Reference.

TECHNICAL BLOCK
Cinematic photoreal, ultra filmic early-1900s harbor pier. 21:9 — 35mm anamorphic film character: horizontal squeeze and compression, oval elliptical bokeh with stretched highlights, curved barrel edge distortion, subtle chromatic aberration toward the edges — all subtle and moderate; NO lens flares, NO streaks, NO floating bokeh circles. SOFT COLOR GRADE: gentle muted warm pastels, cream highlights, lifted soft blacks, low contrast — soft and faded. Shot on 35mm, fine period grain, hazy warm backlit daylight, sun soft behind the drifting funnel smoke — light identical first frame to last. Natural 180-degree shutter cadence — directional motion blur on fast moves, readable poses at holds; no ghosting, no duplicated limbs, no strobing. ALL HANDHELD, organic, never chaotic; SHOT 1 rides a smooth airborne glide, SHOT 10 a static level profile stage. SFX only.

PACE: every shot lands its beat and cuts. Each cut arrives ON an action, never after it — no dead frames, no held breath at the end of a shot, no slowing down to fill time. The piece runs 18 seconds exactly and ends on the frame described in SHOT 10.

LAWS:
- CAST LOCK: exactly TWO named people exist in this film — @He_cruise and @She_cruise — and each appears exactly ONCE in any frame, never doubled, never twinned in the crowd. SHOT 1 contains neither. SHOTS 2, 3 and 4 contain both. From SHOT 5 through SHOT 8 the only named person anywhere on the pier is @He_cruise; every other figure is an anonymous passer-by. SHOT 9 contains @She_cruise alone. SHOT 10 contains @He_cruise alone. Everyone else in every shot is an unnamed period traveller or dockworker.
- LOCATION LOCK: every pier shot — SHOTS 1–8 and SHOT 10 — is the SAME @cruise pier: plank geometry, the crates and barrels on the LEFT, the A-frame cranes on the RIGHT, the beacon tower, the telegraph lines, the moored masts, the funnel smoke and the haze identical throughout; SHOT 9 is the deck of that same liner.
- LIGHT CONTINUITY: ONE single sun serves all ten shots. It sits low and far behind the liner at the deep end of the pier, softened by drifting funnel smoke and sea haze, so every pier shot is BACKLIT from the ship end: bright hazy sky at depth, warm rim light drawing hair, shoulders and cloth edges, faces carried by soft bounce off the pale planks. This exact key survives every hard cut — the same brightness, the same warmth, the same haze density in the first frame of SHOT 1 and the last frame of SHOT 10. SHOT 9 holds that same sun in that same place, now seen from the ship's deck.
- FACE RULE: a lead's visible face fills at least a THIRD of frame height, chest-up or closer, fully inside the frame; at distance a lead reads from behind or in profile; eyes never look into the lens.
- LIVING FACE: the eyes always WORK at a task, never pose; a blink lands ONLY on a change of thought, slow and soft, a few per shot; never a frozen glassy stare. His eyes stay DRY throughout.
- IDENTITY UNDER MOTION: both faces stay photoreal, sharp and identity-locked in EVERY frame, and the same bone structure, proportions and features carry through fast motion, directional motion blur and slow motion alike. In the two slow-motion passages — all of SHOT 3, and SHOT 10 from the takeoff — slow motion changes ONLY the speed of time: entering the ramp, all the way through it, and leaving it, each face holds the exact geometry of its reference. Hair, cloth, dust and spray are the things that stretch; the faces are not.
- THE ONE SUITCASE: exactly ONE suitcase exists in this entire film — small, pale canvas, brown leather trim and corner caps. It lives in @She_cruise's RIGHT hand, held low at her side, in every frame she is visible: SHOTS 2, 3 and 4, and resting beside her hand at the rail in SHOT 9. It stays in that same right hand through the collision, through the full turn and through the walk away — it stays in that hand, and there is only ever the one of it. @He_cruise's two hands are EMPTY in every frame of every shot, the vault and the leap included.
- THE HAT STAYS ON: @She_cruise's wide-brimmed pale hat is fixed on her head and reads level in every frame she appears — through the collision in SHOT 2, the full turn in SHOT 3, the hurried walk away in SHOT 4 and the sea wind at the rail in SHOT 9. The wind moves her hair and the ribbon ends; the hat itself keeps its position on her head.
- NO GLOW: no halo — sun draws only a thin realistic edge; shadow sides honest, never crushed.
- PORTRAIT DISTANCE: close framings come from a longer lens at distance — natural proportions, no wide-angle facial distortion.
- LEVEL CAMERA: horizon level, zero roll, verticals upright.
- LIVING CROWD: the pier carries a dense continuous river of period travellers and dockworkers in every pier frame, and the density is the SAME at 0.0s as at 18.0s — SHOT 1's aerial and SHOT 10's last planks are as full as SHOT 2. At any moment at least a dozen anonymous figures are visible and every one of them is MID-ACTION: embraces, trunks hauled, children darting, hand-carts pushed, a fish crate trailed by gulls. Every body is solid and whole, and every face is a different face. The crowd's attention belongs entirely to its own business — the leads pass through it unnoticed. A passer-by wipes close past the lens every few seconds.
- SHIP & CHAIN (SHOT 10): the liner is UNDER WAY, sliding away past the pier's end, nothing connecting it to shore; the anchor chain hangs as ONE FREE VERTICAL LINE from the bow hawse straight down to the water, plumb, winched slowly up, links rising wet — oval links about thirty centimeters, forearm-thick, the chain narrower than his shoulders; no hooks, never diagonal, never touching the pier. The sea is fine irregular chop — flickering sparkle, wobbling reflections, no stripes, no patterns, never glassy.

SHOT 1 — 0.0–2.5s — ARRIVING WITH THE GULL — geography — aerial glide
100° diagonal field of view, wide rectilinear; this shot's job is geography, so the crowd reads through bodies and movement rather than faces.
First frame: a single white GULL already sweeping in beside the camera against open sky and haze — the camera FLIES WITH IT and drops immediately, descending at 45 degrees over the packed artery of the @cruise pier: the passenger current flowing toward the glowing liner, crates heaved along the edges, a hand-cart cutting a seam, children darting. One continuous descent, no hover. The gull banks away over the crowd exactly as the shot cuts. No lead identifiable.

SHOT 2 — 2.5–3.5s — THE HIT
Hard cut, shoulder height, close, 75° diagonal field of view — both bodies pass within a meter of the lens, so they fill the frame edge to edge. STAGING: @He_cruise enters frame-RIGHT (x≈70%) moving screen-left AGAINST the passenger current, chest and face toward HER; @She_cruise enters frame-LEFT (x≈30%) moving screen-right WITH the current toward the ship, chest and face toward HIM, suitcase in her RIGHT hand — both within one meter of the lens, chests facing each other, never backs. Within half a second their right shoulders CLIP beside the camera: the suitcase swings wide once, her cape flares, his torso twists a quarter-turn, one faltered half-step each, a sharp intake of breath. Momentum carries each ONE step past the other. Cut on the step past.

SHOT 3 — 3.5–6.5s — THE FULL TURN — HE FALLS IN LOVE — slow motion
Hard cut, whole shot in smooth slow motion, chest height, 47° diagonal field of view from about 2.5 m out — a tight 2.39:1 two-shot holding both of them, each face close to a third of frame height, sharp and identity-locked through every frame of the slow motion. STAGING: the pass has put @He_cruise frame-LEFT (x≈35%) and @She_cruise frame-RIGHT (x≈65%), one meter of hazy air between them; BOTH stop and pivot FULLY toward each other on planted feet until they stand squared face to face, the crowd parting around them like water around stones.
ACTING TASK — HIM (the work happens in his eyes): task — understand what just happened to him; he checks both her eyes, one then the other, reading whether she noticed him noticing — and mid-look the guard drops, the eyes soften and widen a fraction, the breath forgets itself: he is FALLING IN LOVE right here and doesn't know the word for it yet. New, helpless, disbelieving.
ACTING TASK — HER: task — measure the stranger one beat past polite and decide he's worth a smile; she holds his eyes, then a real soft smile blooms — given freely, no promise attached. His answering smile arrives one beat AFTER hers. One soft slowed blink from her across the whole shot.
Golden backlit dust hangs; strands of her hair float across her cheek in the rim light; her cape settles in floating folds.

SHOT 4 — 6.5–7.5s — SHE GOES WHERE SHE WAS GOING
Hard cut, real speed crashes back. Camera just behind @He_cruise's right shoulder, his jaw soft in the near-right foreground: @She_cruise breaks the look FIRST — turns and hurries on the way she was always going, toward the ship, back to camera, suitcase in her RIGHT hand, weaving through gaps; passers-by wipe the near foreground and after each wipe she is farther, her hat sinking between shoulders. She does not look back. He stays planted, turned after her. Cut as the crowd takes her.

SHOT 5 — 7.5–10.0s — HE WATCHES HER GO — the turn — his back to the ship — the heavy walk
Hard cut: close slight three-quarter on @He_cruise from ahead-right, 47° diagonal field of view (a 50mm-equivalent long-normal look) from a few meters back, the living crowd soft at every depth.
ACTING TASK — HIM: scene direction (the whole pier lives it): partings are the way of things — you let people go and get on with your day. His motive: a grown man doesn't chase a stranger's ship. His goal: file the encounter away and walk off clean. His tactic — hold the empty spot half a breath where the crowd took her, one slow blink exactly on the loss — then the head DIPS, he TURNS and walks with his BACK to the liner, slowly toward the camera, eyes down, working the planks like a man re-checking an arithmetic that won't come out even; each step re-closes the drawer and each step it swings back open; his pace loses a fraction per step. The camera back-tracks glued to the framing. Three, at most four steps — the beat is the decision to walk away, not the walk.

SHOT 6 — 10.0–12.0s — THE HORN — HE REALIZES — THE PIVOT
Same setup, tighter: @He_cruise's face large in three-quarter, the liner far behind his back. A LONG DEEP HORN rolls over the pier — and the truth gap CLOSES: he has fallen in love, and he is LOSING her, right now, to that ship. His step DIES mid-stride; the eyes SNAP up off the planks and go still — breath catches and shortens, jaw sets, one knocked-out blink; a half-turn of the head back toward the ship — the eyes sharpen, decided — he WHIPS around over his left shoulder, head snapping first with real motion blur, body following on a planted boot, coat flaring — and explodes into the sprint after her. The face never performs — the task does everything. Cut on the explosion into the sprint.

SHOT 7 — 12.0–13.0s — THE CART VAULT — head-on
Hard cut. LOW back-tracking in front of @He_cruise, knee height, 84° diagonal field of view, real speed — a loaded HAND-CART cuts across from frame-left, its puller leaning into the shafts — he does NOT slow: two loading strides, he LEAPS, one palm tapping the top crate, boots swinging clean OVER the load toward the camera, coat flying, dust bursting as he LANDS without breaking stride — the cart-man's head snaps after him. Cut on the landing stride.

SHOT 8 — 13.0–14.0s — THE RUN — close three-quarter
Hard cut. Camera back-tracks fast ahead-right of him, 47° diagonal field of view (a 50mm-equivalent long-normal look): @He_cruise's face LARGE in three-quarter, breath tearing, arms driving, jaw carved by the backlight; the crowd blurs behind his far shoulder, heads turning after him.
ACTING TASK: his eyes CATCH THE SHIP beyond the camera and hold it like a rope — measuring the shrinking distance stride by stride, recalculating the path through the crowd without letting go.

SHOT 9 — 14.0–15.0s — HER PROFILE AT THE RAIL — lightly nonchalant
Hard cut — high on the ship, 29° diagonal field of view (an 85mm-equivalent look) from well back along the deck: @She_cruise alone in frame, her clean PROFILE close-up at the deck rail against the open hazy golden sky, hat line sharp against the light, sea wind lifting her hair back, one hand resting easy on the worn rail.
ACTING TASK — HER: task — appraise the running stranger below without giving anything away; her eyes angle DOWN to the pier and snag on the running figure: a soft huff of an exhale through the nose, the visible corner of her mouth curling into a faint warm half-smile, one slow amused narrowing of the eye — cool on the surface, a spark of real interest underneath. One soft blink on the moment she notices. No big emotion. Cut on the half-smile.

SHOT 10 — 15.0–18.0s — THE FINAL SPRINT AND THE LEAP — static profile
Hard cut. One breath of the sprint from behind-side: the last bare planks of the @cruise pier rushing under @He_cruise's boots, the towering dark bow sliding AWAY past the pier's end, the wet anchor chain climbing link by link out of the choppy water — he BURSTS, head dropping into full commitment — then the STATIC profile stage, 84° diagonal field of view, level, perpendicular, 1.5 meters above the water. STAGING: the pier edge lower-LEFT with its empty planks; the vertical chain at x≈65% falling from the top edge to the wet anchor at the water (y≈75%); the dark hull edge slicing the extreme frame-RIGHT, gliding away; cranes and moored ships far in the haze; his flight crosses the CENTER at the upper third, left to right. His last stride PLANTS on the edge plank — it flexes, dust kicks — and EXACTLY on the takeoff the shot ramps into smooth slow motion: one short committed spring over the choppy gap, body stretching fully HORIZONTAL like a swimmer, coat flaring behind, one white gull gliding above the arc — both arms driving forward, hands OPEN, fingers spreading toward the wet links just above the anchor — closer — centimeters. Final frame HELD: full wide profile, mid-air at the top of the reach, hands a palm's width from the rising iron, spray suspended, the gull mid-wingbeat. This exact frame is the last frame of the film, at 18.0s.

PHYSICS
Both leads hurry with real weight; the collision has true mass — torsos rotate, one faltered half-step each, never backward; the full turns pivot on planted feet. His heavy walk carries honest weight; the dead stop lands on a planted heel; the whip pivot turns head-first with motion blur; the sprint explodes through real acceleration and holds full tilt. The vault is true parkour physics — the palm takes real weight, hips swing clean, the landing leg rolls back into stride, the cart rocks, nothing tips. The leap is true ballistics — one driving-leg takeoff, a shallow rising arc, no floating, no mid-air correction; slow motion stretches time, never the trajectory. The chain hangs plumb under its own weight, rising at a steady winch pace, shedding scattered heavy drops. Cloth, hair and the suitcase carry natural delay, exaggerated only in slow motion; every figure solid; the gull flies with real wing mechanics.

SFX only: from the air — wind, the gull's wingbeats and one cry, the dock's wash rising with the descent — dropping INTO the crowd: layered boots, urgent murmur, crate thuds, cart wheels, gulls; the soft cloth THUD of the hit, a sharp intake of breath — the world dipping muffled through the slow-motion turn, one slowed gull cry, a single close breath of each — crashing back as she leaves; farewells washing under his held breath, his slowing steps — the LONG DEEP HORN over everything, one caught breath in the near-silence, the whip of cloth on the pivot — the rapid hammer of the sprint, grinding cart wheels, the slap of his palm, the double thud of the vault — one beat of high sea wind and her amused exhale at the rail — boots on the last planks, the plank's creak under the plant, the deep wash of the moving hull, the rhythmic clank of the windlass — and the world falls away with the leap into a soft airy hush: wind, heartbeat, one slowed gull cry, water flicking off the rising links — the piece ends inside the held breath of the reach. No dialogue, no music. 35mm grain, soft faded period-drama realism.

POSITIVE CONSTRAINTS (the ten things that must be true of every frame)
1. One @He_cruise and one @She_cruise, each matching its reference exactly, each appearing once.
2. Both faces sharp and identity-locked at every frame, at every speed, in both slow-motion passages.
3. One suitcase, in her right hand, low at her side, every frame she is visible.
4. Both his hands empty, every frame.
5. Her hat on her head and level, every frame she is visible.
6. One sun, low behind the liner at the pier's deep end, backlighting every pier shot — identical across all ten.
7. The same @cruise pier in every pier shot: crates and barrels left, A-frame cranes right, beacon tower, telegraph lines, moored masts.
8. A packed pier at the same density from the first frame to the last, every anonymous figure mid-action, every face a different face.
9. Horizon level, verticals upright, zero roll.
10. Diegetic sound only.
```

---

## הסצנות הבאות

הפרומפטים לסצנות 2–6 ייכתבו כשנגיע אליהן, אחרי שנראה איך הפנים שלכם מתנהגות
בסצנת הנמל. תגיד לי איזו סצנה — אני מריץ את `higgsfield-seedance-prompt-builder`
ומחזיר פרומפט מלא באותו מבנה.

**כלל הזהב לכל סצנה חדשה:** להעתיק את ה-TECHNICAL BLOCK מסצנה 1 מילה במילה.
אותה אופטיקה, אותו גרעין, אותו גרייד — זה מה שהופך שש תקופות לסרט אחד.


---

# סצנה 5 — הקרנבל

**מודל:** Seedance 2.5 · **יחס:** 21:9 · **רזולוציה:** 1080p · 9 שוטים, 18 שניות
**Elements:** `@He_carnival` · `@She_carnival` · `@carnival`
**קבצים:** [`scene5-carnival.prompt.txt`](scene5-carnival.prompt.txt) ·
[`he-carnival-sheet.prompt.txt`](he-carnival-sheet.prompt.txt) ·
[`she-carnival-sheet.prompt.txt`](she-carnival-sheet.prompt.txt)

## איך הסצנה בנויה

שלושת הביטים של המדריך (החיפוש · המצעד · הנגיעה) מכווצים לגנרציה אחת של 18 שניות,
באותו טווח אורך של סצנת הנמל. ביט המצעד — ההפרדה על ידי מצעד וכרכרה — הושמט: הוא
סיבוך, לא קשת. החיפוש והנגיעה לבדם הם סיפור שלם.

| שוט | | אורך |
|---|---|---|
| 1 | היציאה מהארמון — גיאוגרפיה, הרעש מכה בו | 2.5s |
| 2 | ההצצה — בורדו בוער דרך פרצה בקהל | 1.5s |
| 3 | הוא יורד לתוך ההמון | 1.5s |
| 4 | היא נעה דרכו, לא ממהרת | 1.5s |
| 5 | הקרנבל תופס אותו — מעגל רוקדות מסובב אותו | 2.0s |
| 6 | הוא מאבד אותה — עצירה, סריקה | 2.0s |
| 7 | **המבט האחד שלה** — סלואו-מושן | 2.0s |
| 8 | המטרים האחרונים + הכפפה יורדת | 2.0s |
| 9 | **הנגיעה** — סלואו-מושן, פריים אחרון מוחזק | 3.0s |

## שלוש החלטות שכדאי להכיר

**המסכות צרות וגבוהות.** במדריך המקורי שניהם עם מסכות ונציאניות מלאות. מסכת עיניים רגילה
מכסה גבות וחלק מעצמות הלחיים — בדיוק אזור הזהות. בשני דפי הדמות ובפרומפט הוידאו נעולה
מסכה שהיא **פס צר וגבוה סביב פתחי העיניים בלבד**: הגבות נקראות מעליה, האף, הלחיים, הפה
והלסת חשופים לגמרי. הקוד הוויזואלי של הנשף נשמר, הפנים נשמרות.

**הבורדו הוא מנגנון עלילתי, לא החלטת סטיילינג.** חוק `THE ONLY BURGUNDY` קובע שבכל
הכיכר בדיוק שני אנשים לובשים אדום או בורדו — והם השניים. כל השאר באזמרגד, ספיר, סגול,
טורקיז, זהב, כסף, אוכרה או קרם. זה מה שמאפשר לשוט 2 לעבוד: הוא מוצא אותה לפי צבע.
בלוח הלוקיישן שכבר כתבנו יש `ABSOLUTELY no red, no burgundy` בקהל — זה בכוונה.

**אין מוזיקה, בשום מקום.** המדריך חוזר על זה יותר מפעם אחת, ובצדק: קרנבל מזמין את המודל
לייצר תזמורת. במקום זה הכיכר **רועשת** — קולות, צחוק, מחיאות כפיים, נפצים, פעמוני ליצנים,
להבות לפידים. אם מוזיקה בכל זאת נכנסת, זה הכשל הראשון לבדוק.

## הרצה

1. `@He_carnival` ו-`@She_carnival` — Seedream 5.0 Pro, 16:9, 2K, עם תמונות הרפרנס האמיתיות
2. מחיקת פנים מפאנלי הגוף המלא + היברידיזציה על הקלוז-אפ
3. `@carnival` — Soul Cinema, 16:9, 1080p. הפרומפט ב-[`01-stage1-asset-prompts.md`](01-stage1-asset-prompts.md) סעיף 1.4
4. העלאה ל-Elements בשמות המדויקים
5. הפרומפט ל-Cinema Studio


---

# סצנה 2 — הזירה

**מודל:** Seedance 2.5 · **יחס:** 21:9 · **רזולוציה:** 1080p · 9 שוטים, 18 שניות
**Elements:** `@He_arena` · `@She_arena` · `@robot` · `@arena` · `@sketch`
**קבצים:** [`scene2-arena.prompt.txt`](scene2-arena.prompt.txt) ·
[`he-arena-sheet-tagged.prompt.txt`](he-arena-sheet-tagged.prompt.txt) ·
[`she-arena-sheet-tagged.prompt.txt`](she-arena-sheet-tagged.prompt.txt) ·
[`robot-sheet.prompt.txt`](robot-sheet.prompt.txt) ·
[`arena-plate.prompt.txt`](arena-plate.prompt.txt) ·
[`arena-sketch.prompt.txt`](arena-sketch.prompt.txt)

## שני שינויים מבניים מול המדריך

**איחוד שתי הגנרציות לאחת.** במקור: 2a הקרב שלו (25s), 2b התא שלה (25s). בנפרד, האחת היא
סרט אקשן בלי אישה והשנייה סצנת ריאקשן בלי קרב. משולבות ב-18 שניות זה מה שהסצנה באמת —
הוא נלחם, היא לא יכולה להראות שאכפת לה, ובסוף היא נותנת לו מבט אחד. חוסך גם גנרציה שלמה.

**`@throne` מתמזג ל-`@arena`.** לוח לוקיישן אחד שכולל את הלוג'יה המלכותית עם הכס המוזהב —
נכס אחד פחות, וגם נעילת מיקום טובה יותר: הלוג'יה תמיד באותו מקום ביחס לחומה.

## סדר השוטים

| שוט | | אורך |
|---|---|---|
| 1 | העמידה מול — יחס הגודל, **והמכונה מרימה את הלהב ראשונה** | 1.5s |
| 2 | היא בתא, קרירה, לא נותנת כלום | 1.0s |
| 3 | **המכה הראשונה** — הלהב יורד, הוא מתגלגל, החול נחפר | 1.5s |
| 4 | **הוא מנסה ונכשל** — חותך בברך, ניצוצות, המכונה לא מגיבה | 1.5s |
| 5 | **הוא חוטף** — גב-יד מעיפה אותו לחומה. הוא לא קם | 1.5s |
| 6 | היא כמעט מסגירה את עצמה — עכשיו יש לזה סיבה | 1.5s |
| 7 | **הוא קם** — אבק, רצועה קרועה, המכה השנייה, והוא עולה מהמכתש | 1.5s |
| 8 | הטיפוס — המכונה **נאבקת**, מסתובבת, מנסה להגיע לגב שלה | 2.0s |
| 9 | ההטלה — היא נופלת, האדמה קופצת | 2.0s |
| 10 | החרב, ואז הוא מרים עיניים אליה | 2.0s |
| 11 | **המבט האחד שלה** — סלואו-מושן, פריים מוחזק | 2.0s |

**חלוקת הזמן:** 13.5 שניות קרב, 4.5 שניות שלה.

## שלוש נעילות שהסצנה תלויה בהן

**יחס 1:3.** זה החוק הראשון בפרומפט ובעל הניסוח הכי חד: `the top of @He_arena's head reaches
only @giant's MID-THIGH — not his waist, not his chest`. מודלי וידאו מושכים הפרשי גודל
קיצוניים חזרה לפרופורציות סבירות, ובלי מספר ונקודת בדיקה אנטומית זה קורה בכל טייק.
`@sketch` קיים בדיוק בשביל זה, ובפרומפט כתוב במפורש שהוא שולט **רק** ביחס הגודל —
לא בסגנון הקו, לא ברקע הלבן ולא בפוזות.

**אין פנים.** ל-`@robot` יש לוח ברונזה חלק ושני חריצי עיניים, וזהו. זה גם עיצוב וגם הגנה:
פנים שהמודל ממציא בכל שוט הן פנים שמשתנות בכל שוט.

**הוא לא מסיים.** הקרב נגמר עם המכונה שטוחה על הגב, הליבה חשופה, והחרב נמוכה. אין דם,
אין חלקים תלושים, אין פיצוץ ואין אש. מעבר לטעם — זה מה שמשאיר אותו דמות שאפשר
להתאהב בה בשש תקופות.

**האנכרוניזם הוא הכוונה, וזה כתוב במפורש.** מכונה מהעתיד הרחוק בזירת אבן עתיקה — אם לא
מסמנים את זה ככוונה, המודל "מתקן": או שהוא מרכך את המכונה, או שהוא ממדרן את הזירה.
לכן בהגדרת הרפרנס כתוב `It does not belong to this place or this century, and it is not
supposed to`, ובחוק התאורה נקבע שהזוהר הציאני שלה הוא **מקור האור הקר היחיד בסרט**,
שלא מאיר את החול ולא משנה את השמש. אבן חמה מול מכונה קרה — זו התמונה.

**שלושת המנגנונים שהקרב תלוי בהם נשמרו** למרות ההחלפה: יחס 1:3, מבנה גב שאפשר לטפס
עליו (עמוד שדרה של צרורות אקטואטורים וכבלים, עם מדרגות שריון משני צדדיו), וכובד עליון
שהופך את ההטלה לאפשרית.

**המכונה תוקפת, היא לא מטרה.** בגרסה הראשונה היא עמדה במקום עד ההטלה — זו לא הייתה
לחימה. עכשיו יש חוק `THE MACHINE FIGHTS BACK`: היא זזה ראשונה, מורידה להב פעמיים,
מעיפה אותו לחומה בגב-יד, ונאבקת בו בזמן הטיפוס. האיטיות שלה היא **מחויבות**, לא פסיביות —
`once a swing starts it cannot be recalled`, וזמן ההתאוששות מהמכה הוא הפתח היחיד שיש לו.

**הוא סופג נזק אמיתי.** חוק `HE TAKES REAL DAMAGE`: משוט 7 והלאה הוא נושא את זה — נשימה
כבדה, אבק בכתף ובצד הפנים, רצועת baldric קרועה, יד שמאל מוחזקת קרוב. בלי דם ובלי פצעים;
הנזק נקרא דרך אבק, עור קרוע ואיך שהוא נושא את עצמו. וזה גם מה שנותן לשוט שלה משמעות —
היא כמעט מסגירה את עצמה **כי הוא בדיוק ספג מכה**, לא סתם.


---

# סצנה 3 — הרכבת

**מודל:** Seedance 2.5 · **יחס:** 21:9 · **רזולוציה:** 1080p · 10 שוטים, 18 שניות
**Elements:** `@He_train` · `@She_train` · `@train` · `@boards`
**קבצים:** [`scene3-train.prompt.txt`](scene3-train.prompt.txt) ·
[`he-train-sheet-tagged.prompt.txt`](he-train-sheet-tagged.prompt.txt) ·
[`she-train-sheet-tagged.prompt.txt`](she-train-sheet-tagged.prompt.txt) ·
[`train-plate.prompt.txt`](train-plate.prompt.txt) ·
[`train-boards.prompt.txt`](train-boards.prompt.txt)

## ההיפוך שעושה את הסצנה

בכל שאר התקופות הוא רודף והיא נלקחת ממנו — האונייה לוקחת אותה, הקהל בולע אותה.
כאן **היא הפושעת**, היא שולטת, והיא לוקחת את עצמה: צוללת מגשר לים ונעלמת מרצונה.
הוא עושה הכל נכון ומגיע **שנייה אחת מאוחר מדי**. זו התקופה היחידה שבה מה שמפריד ביניהם
זו ההחלטה שלה.

| שוט | | אורך |
|---|---|---|
| 1 | המרדף — הרכבת חוצה את המישור, שני רוכבים לצידה | 2.0s |
| 2 | הוא עולה — קפיצה מהאוכף למעקה הקרון האחרון | 1.5s |
| 3 | מתיחה לגג — משיכה אמיתית, ואז הרוח | 1.5s |
| 4 | **היא מטפלת בזה** — הטלה נקייה אחת, והיא אפילו לא מיישרת את הכובע | 2.0s |
| 5 | הקרב שלו — שני שומרים, שליפה עצורה, שניהם על הרצפה | 2.0s |
| 6 | הוא רואה אותה — שלושה קרונות קדימה, היא מסתובבת אליו | 1.5s |
| 7 | **הגשר** — הרכבת יוצאת אל מעל הים | 1.5s |
| 8 | היא הולכת לקצה — ומביטה בו פעם אחת | 2.0s |
| 9 | **הצלילה** — סלואו-מושן, נפילה ארוכה, נתז אחד | 2.0s |
| 10 | הוא מגיע לגג הריק ומחפש בים. פריים מוחזק | 2.0s |

## שלוש הנעילות

**כיוון מסך.** זה החוק הראשון, ובצדק — סצנות רכבת נכשלות בזה יותר מבכל דבר אחר. הרכבת
נוסעת **שמאל לימין תמיד**, הקטר תמיד לכיוון ימין הפריים, וכל תנועה שלו קדימה היא ימינה.
זה מה ש-`@boards` פאנל 1 נועל.

**מפת הגגות.** היא על הקרון השלישי, הוא על הרביעי, קרון אחד ביניהם, וכל הגגות **מאחוריו
ריקים**. בלי זה המודל ממלא את הרכבת בשומרים בכל שוט.

**אף אחד לא יורה ואף אחד לא מת.** שני האקדחים נשארים בנרתיק כל הסרט; שני הקרבות ידיים
בלבד; שני השומרים מסיימים מחוסרי הכרה ושלמים. אין דם. זה עקבי עם הזירה, וזה גם מונע
מהמודל להפוך את זה לתחרות ירי.

## הערה על הסטוריבורד

במדריך המקורי היו שלושה סטוריבורדים נפרדים. איחדתי לגיליון אחד עם שלושה פאנלים —
כיוון מסך, מפת גגות, גיאומטריית הגשר. בפרומפט כתוב במפורש ש-`@boards` שולט
**רק בשלושת אלה**: `Its pencil line style, its white paper and its framing never appear in
the film`. בלי הצמצום תקבל וידאו שנראה כמו סקיצת עיפרון.


---

# סצנה 7 — המטבח (הסיום)

**מודל:** Seedance 2.5 · **יחס:** 21:9 · **רזולוציה:** 1080p · 8 שוטים, 18 שניות
**Elements:** `@He_kitchen` · `@She_kitchen` · `@kitchen` · `@bowl`
**קבצים:** [`scene6-kitchen.prompt.txt`](scene6-kitchen.prompt.txt) ·
[`he-kitchen-sheet-tagged.prompt.txt`](he-kitchen-sheet-tagged.prompt.txt) ·
[`she-kitchen-sheet-tagged.prompt.txt`](she-kitchen-sheet-tagged.prompt.txt) ·
[`kitchen-plate.prompt.txt`](kitchen-plate.prompt.txt) ·
[`bowl-plate.prompt.txt`](bowl-plate.prompt.txt)

## זו לא סצנת סיום — זו סצנת התחלה

הסרט נגמר איפה שהסיפור האמיתי התחיל: מטבח מקצועי באמצע שירות, היא מפגרת עם הזיתים,
היא הולכת לאורך הקו וניגשת אליו לבקש עזרה. אחרי חמש תקופות של הושטות יד בלתי אפשריות —
קפיצה אחרי אונייה, כיכר של אלף איש, גשר מעל הים — מה שבאמת מתחיל את זה הוא אישה עם
קערת זיתים ששואלת זר אם הוא יכול לעזור.

| שוט | | אורך |
|---|---|---|
| 1 | שירות — הקו בשיא, שניהם בו, לא מכירים | 2.5s |
| 2 | היא מפגרת — טונה של זיתים ושלושה דברים על האש | 2.0s |
| 3 | היא הולכת לאורך הקו עם הקערה | 2.0s |
| 4 | **הבקשה** — היא מרימה את הקערה ושואלת | 2.5s |
| 5 | הוא אומר כן — מנגב ידיים, לוקח, מפנה מקום | 2.0s |
| 6 | ארבע ידיים בקערה — **הנגיעה** | 2.5s |
| 7 | **המבט הראשון** — היא מציצה, הוא כבר מסתכל | 2.0s |
| 8 | רחב — שניים כפופים מעל קערה בתוך מטבח שואג | 2.5s |

## השורה — הדיבור היחיד בכל הסרט

היא אומרת משפט אחד, באנגלית, בשוט 4:

> **"Can you help me with the olives?"**

**אם המשפט האמיתי שלה שונה** — תחליף אותו בשני המקומות שהוא מופיע בפרומפט
(חוק `THE ASK` ושוט 4) ואל תיגע בשום דבר אחר.

שלוש החלטות סביבו:

**רק היא מדברת.** הוא עונה בניגוב ידיים ובלקיחת הקערה, ולא אומר מילה בשום מקום בסרט.
שורה אחת בסרט שלם היא נשק; שתיים זה כבר דיאלוג, וכל שורה נוספת היא עוד סיכון ליפ-סינק.

**החדר יורד מתחתיה.** בבלוק האודיו: `the whole room ducks a little under her voice so the
words read clearly, and comes straight back up`. בלי זה השורה נבלעת במטבח.

**קולות הבריגדה נשארים בלתי מובנים** — `NEVER intelligible, not one word you can make out`.
זה מה שמונע מהמודל להמציא דיאלוג בכל פינה של החדר.

**תזמון:** שוט 4 הועלה ל-3 שניות כדי שהשורה תיכנס בנוחות — הגעה 0.5s, שורה 1.7s,
והוא מחזיק מבט 0.8s. הזמן הגיע משוט 1.

## ארבעה חוקים שנושאים את הסצנה

**`THEY DO NOT KNOW EACH OTHER`** — הם זרים. אין ברכה, אין נגיעה על היד, אין בדיחה
משותפת, אין שפת גוף נוחה, ואין טבעות. הכל קורה בפעם הראשונה.

**`THEY NEVER STOP WORKING`** — אף אחד מהם לא עוצר. הידיים ממשיכות דרך הבקשה, דרך
התשובה, דרך הנגיעה ודרך המבט. `Nothing in this scene is important enough to put a knife
down for — that is exactly why it matters.`

**`THE TOUCH THEY DO NOT NOTICE`** — האצבעות נפגשות על שפת הקערה. אותה נגיעה שהסרט
רדף אחריה חמש תקופות. אף אחד לא מסתכל למטה. אסור לדרמטיזציה.

**אין סלואו-מושן.** בכל תקופה אחרת יש רמפה לרגע הגדול. כאן אין — כי אף אחד בחדר,
כולל הם, לא יודע שקרה משהו.

## המטבח המקצועי מחזיר את דקדוק הסרט

הגרסה הביתית הייתה שקטה, וזה שבר את התבנית. בכל תקופה אחרת יש **עולם רועש ושני אנשים
שמוצאים בו שקט**: רציף צפוף, כיכר צפופה, זירה שואגת, רכבת ורוח. מטבח בשירות מחזיר
בדיוק את זה — וחוק `LIVING BRIGADE` הוא המקבילה ל-`LIVING CROWD` של הנמל והקרנבל.
