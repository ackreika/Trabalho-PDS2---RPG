shader_type canvas_item;

uniform vec4 madeira_clara : source_color = vec4(0.48, 0.29, 0.15, 1.0);
uniform vec4 madeira_media : source_color = vec4(0.31, 0.17, 0.09, 1.0);
uniform vec4 madeira_escura : source_color = vec4(0.18, 0.09, 0.05, 1.0);

void fragment() {
	vec2 uv = UV;
	float ondulacao = sin(uv.y * 54.0 + sin(uv.x * 9.0) * 1.8 + sin(uv.x * 37.0) * 0.28);
	float veio_fino = sin(uv.y * 180.0 + sin(uv.x * 16.0) * 0.9);
	float veio = smoothstep(0.58, 0.96, ondulacao * 0.72 + veio_fino * 0.28);
	float tabua = smoothstep(0.985, 1.0, fract(uv.y * 3.0));
	float tom = clamp(0.40 + ondulacao * 0.13 + veio * 0.15, 0.0, 1.0);
	vec4 cor = mix(madeira_escura, madeira_media, smoothstep(0.05, 0.78, tom));
	cor = mix(cor, madeira_clara, veio * 0.32);
	cor = mix(cor, madeira_escura, tabua * 0.72);
	COLOR = vec4(cor.rgb, cor.a * COLOR.a);
}
