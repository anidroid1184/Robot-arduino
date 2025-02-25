// No es necesario incluir S o f t w a r e S e r i a l en el Arduino Mega
2 // # include < S o f t w a r e S e r i a l .h>
3
4 // D e f i n i c i n de comandos con macros
5 # define FORWARD ’F’
6 # define BACKWARD ’B’
7 # define turnLeft ’L’
8 # define turnRight ’R’
9 # define AUTO ’A’ // Modo a u t o m t i c o
10 # define MANUAL ’M’ // Modo manual
11 # define SPEED_UP ’U’ // Aumentar velocidad
12 # define SPEED_DOWN ’D’ // Disminuir velocidad
13
14 // Pines de los motores
15 const int in1 = 25;
16 const int in2 = 26;
17 const int in3 = 27;
18 const int in4 = 28;
19 const int enA = 5; // Pin PWM para el motor A
20 const int enB = 6; // Pin PWM para el motor B
21
22 // Pines del sensor u l t r a s n i c o
23 const int trigPin = 30;
24 const int echoPin = 31;
25
26 // Variables para control
27 char command ;
28 int speed = 100; // Velocidad inicial del motor
29
30 // Variables para el modo de control
31 bool isManualMode = true ; // Inicia en modo manual
32 bool autoModeActive = false ; // Variable para controlar el modo
,→ a u t o m t i c o
33
34 // Variables para la m q u i n a de estados en modo a u t o m t i c o
35 int autoState = 0; // Estado actual del modo
,→ a u t o m t i c o
36 unsigned long lastStateChangeTime = 0; // Tiempo del ltimo
,→ cambio de estado
37
38 void setup () {
39 // Configurar los pines de los motores
40 pinMode ( in1 , OUTPUT );
41 pinMode ( in2 , OUTPUT );
42 pinMode ( in3 , OUTPUT );
43 pinMode ( in4 , OUTPUT );
44 pinMode ( enA , OUTPUT );
45 pinMode ( enB , OUTPUT );
46
47 // Configurar los pines del sensor u l t r a s n i c o
48 pinMode ( trigPin , OUTPUT ) ;
49 pinMode ( echoPin , INPUT );
50
51 // Inicializar c o m u n i c a c i n serial
52 Serial . begin (9600) ; // C o m u n i c a c i n con el monitor serial

1

53 Serial1 . begin (9600) ; // C o m u n i c a c i n con el m d u l o Bluetooth
54
55 // Inicializar variables de la m q u i n a de estados
56 autoState = 0;
57 lastStateChangeTime = millis () ;
58 }
59
60 void loop () {
61 // Comprobar si se ha recibido un comando por Bluetooth
62 if ( Serial1 . available () ) {
63 command = Serial1 . read () ; // Leer el comando
64 Serial . print (" Comando recibido : ");
65 Serial . println ( command ); // Mostrar el comando recibido
66 controlCar ( command ) ; // Ejecutar la f u n c i n de control
67 }
68
69 // Si estamos en modo a u t o m t i c o , ejecutar la f u n c i n de avance
,→ a u t o m t i c o
70 if ( autoModeActive ) {
71 automaticForward () ;
72 }
73 }
74
75 // F u n c i n para controlar el carro
76 void controlCar ( char command ) {
77 if ( command == AUTO ) {
78 isManualMode = false ; // Cambiar a modo a u t o m t i c o
79 autoModeActive = true ; // Activar el modo a u t o m t i c o
80 autoState = 0; // Reiniciar estado a u t o m t i c o
81 lastStateChangeTime = millis () ;
82 Serial . println (" Modo a u t o m t i c o activado .");
83 }
84 else if ( command == MANUAL ) {
85 isManualMode = true ; // Cambiar a modo manual
86 autoModeActive = false ; // Desactivar el modo a u t o m t i c o
87 Serial . println (" Modo manual activado .");
88 stopCar () ; // Detener el carro al cambiar a

,→ modo manual

89 }
90 else if ( isManualMode ) {
91 switch ( command ) {
92 case FORWARD : // Adelante
93 forward () ;
94 break ;
95 case BACKWARD : // A t r s
96 backward () ;
97 break ;
98 case turnLeft : // Izquierda
99 turnLeftFunc () ;
100 break ;
101 case turnRight : // Derecha
102 turnRightFunc () ;
103 break ;
104 case ’S’: // Stop ( detener el carro )
105 stopCar () ;
106 break ;
107 default :

2

108 stopCar () ; // Detener por defecto
109 break ;
110 }
111 }
112
113 // Comando para ajustar la velocidad
114 if ( command == SPEED_UP ) {
115 increaseSpeed () ; // Aumentar velocidad
116 } else if ( command == SPEED_DOWN ) {
117 decreaseSpeed () ; // Disminuir velocidad
118 }
119 }
120
121 // F u n c i n para avanzar a u t o m t i c a m e n t e y realizar maniobras al
,→ detectar o b s t c u l o s
122 void automaticForward () {
123 unsigned long currentTime = millis () ;
124
125 switch ( autoState ) {
126 case 0: // Avanzando
127 Serial . println (" Avanzando en modo a u t o m t i c o .");
128 forward () ;
129 if ( obstacleDetected () ) {
130 Serial . println (" O b s t c u l o detectado . Deteniendo el carro ."

,→ );
131 stopCar () ;
132 autoState = 1; // Cambia al estado de detenerse
133 lastStateChangeTime = currentTime ;
134 }
135 break ;
136
137 case 1: // Detenido d e s p u s de detectar o b s t c u l o
138 if ( currentTime - lastStateChangeTime >= 500) { // Espera 500

,→ ms

139 Serial . println (" Retrocediendo ... ");
140 backward () ;
141 autoState = 2; // Cambia al estado de retroceder
142 lastStateChangeTime = currentTime ;
143 }
144 break ;
145
146 case 2: // Re tr oc ed ie nd o
147 if ( currentTime - lastStateChangeTime >= 300) { // Retrocede

,→ durante 300 ms
148 stopCar () ;
149 Serial . println (" Detenido d e s p u s de retroceder .");
150 autoState = 3; // Cambia al estado de girar
151 lastStateChangeTime = currentTime ;
152 }
153 break ;
154
155 case 3: // Girando a la derecha
156 if ( currentTime - lastStateChangeTime >= 200) { // Espera 200

,→ ms antes de girar

157 Serial . println (" Girando a la derecha ...") ;
158 turnRightFunc () ;
159 autoState = 4; // Cambia al estado de girar

3

160 lastStateChangeTime = currentTime ;
161 }
162 break ;
163
164 case 4: // Continuar girando
165 if ( currentTime - lastStateChangeTime >= 300) { // Gira

,→ durante 300 ms
166 stopCar () ;
167 Serial . println (" Detenido d e s p u s de girar .");
168 autoState = 5; // Cambia al estado de verificar si el

,→ o b s t c u l o persiste
169 lastStateChangeTime = currentTime ;
170 }
171 break ;
172
173 case 5: // Verificar si el o b s t c u l o persiste
174 if ( currentTime - lastStateChangeTime >= 200) { // Espera 200

,→ ms

175 if ( obstacleDetected () ) {
176 Serial . println (" O b s t c u l o a n presente . Repitiendo

,→ maniobra .");

177 autoState = 1; // Regresa al estado de detenerse
178 } else {
179 Serial . println (" O b s t c u l o evitado . Continuando hacia

,→ adelante .");

180 autoState = 0; // Vuelve al estado de avanzar
181 }
182 lastStateChangeTime = currentTime ;
183 }
184 break ;
185
186 default :
187 autoState = 0;
188 break ;
189 }
190 }
191
192 // F u n c i n para detectar o b s t c u l o s con el sensor u l t r a s n i c o
193 bool obstacleDetected () {
194 long duration , distance ;
195 digitalWrite ( trigPin , LOW );
196 delayMicroseconds (2) ;
197 digitalWrite ( trigPin , HIGH );
198 delayMicroseconds (10) ;
199 digitalWrite ( trigPin , LOW );
200
201 duration = pulseIn ( echoPin , HIGH , 30000) ; // A a d i d o timeout de

,→ 30 ms

202 distance = ( duration * 0.034) / 2;
203
204 Serial . print (" Distancia : ");
205 Serial . print ( distance );
206 Serial . println (" cm");
207
208 if ( distance > 0 && distance < 50) { // Si hay un o b s t c u l o

,→ cerca ( menor a 50 cm)

209 return true ;

4

210 }
211 return false ;
212 }
213
214 // Funciones para el movimiento del carro (NO MODIFICADAS )
215 void turnRightFunc () {
216 Serial . println (" Moviendo hacia a t r s .");
217 digitalWrite ( in1 , HIGH );
218 digitalWrite ( in2 , LOW );
219 digitalWrite ( in3 , HIGH );
220 digitalWrite ( in4 , LOW );
221 analogWrite ( enA , speed ); // Controla la velocidad
222 analogWrite ( enB , speed ); // Controla la velocidad
223 }
224
225 void turnLeftFunc () {
226 Serial . println (" Moviendo hacia adelante .") ;
227 digitalWrite ( in1 , LOW );
228 digitalWrite ( in2 , HIGH );
229 digitalWrite ( in3 , LOW );
230 digitalWrite ( in4 , HIGH );
231 analogWrite ( enA , speed ); // Controla la velocidad
232 analogWrite ( enB , speed ); // Controla la velocidad
233 }
234
235 void forward () {
236 Serial . println (" Girando a la izquierda .");
237 digitalWrite ( in1 , HIGH );
238 digitalWrite ( in2 , LOW );
239 digitalWrite ( in3 , LOW );
240 digitalWrite ( in4 , HIGH );
241 analogWrite ( enA , speed );
242 analogWrite ( enB , speed );
243 }
244
245 void backward () {
246 Serial . println (" Girando a la derecha .");
247 digitalWrite ( in1 , LOW );
248 digitalWrite ( in2 , HIGH );
249 digitalWrite ( in3 , HIGH );
250 digitalWrite ( in4 , LOW );
251 analogWrite ( enA , speed );
252 analogWrite ( enB , speed );
253 }
254
255 void stopCar () {
256 Serial . println (" Deteniendo el carro .");
257 digitalWrite ( in1 , LOW );
258 digitalWrite ( in2 , LOW );
259 digitalWrite ( in3 , LOW );
260 digitalWrite ( in4 , LOW );
261 analogWrite ( enA , 0) ; // Detener el motor A
262 analogWrite ( enB , 0) ; // Detener el motor B
263 }
264
265 // Funciones para ajustar la velocidad ( NO MODIFICADAS )
266 void increaseSpeed () {

5

267 if ( speed < 255) {
268 speed += 25; // Aumenta la velocidad en pasos de 25
269 Serial . print (" Velocidad aumentada a: ");
270 Serial . println ( speed ) ;
271 }
272 }
273
274 void decreaseSpeed () {
275 if ( speed > 0) {
276 speed -= 25; // Disminuye la velocidad en pasos de 25
277 Serial . print (" Velocidad disminuida a: ") ;
278 Serial . println ( speed ) ;
279 }
280 }
