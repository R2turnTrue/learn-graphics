#version 330 core

out vec4 FragColor;
in vec2 texCoord;
in vec3 fragPos;

uniform vec3 viewPos;
uniform vec4 ourColor;

void main()
{
    float size_factor = 10.0;

    float cell_line_thickness    = 0.005 / size_factor / 2;
    float subcell_line_thickness = 0.001 / size_factor / 2;

    //vec2 crd = texCoord + viewPos.xz / 10;
    vec2 crd = texCoord + viewPos.xz / 100;

    vec4 cell_color    = vec4( 0.4, 0.4, 0.4, 0.5 );
    vec4 subcell_color = vec4(  0.25,  0.25,  0.25, 0.5 );

    float grid_size = 1.0;
    float cell_size = 1 / size_factor;
    float half_cell_size = cell_size * 0.5;
    float subcell_size = 0.1 / size_factor;
    float half_subcell_size = subcell_size * 0.5;

    vec2 cell_coords = mod(crd + half_cell_size, cell_size);
    vec2 subcell_coords = mod(crd + half_subcell_size, subcell_size);

    vec2 distance_to_cell = abs(cell_coords - half_cell_size);
    vec2 distance_to_subcell = abs(subcell_coords - half_subcell_size);

    vec4 color = vec4(0.0f);

    vec2 d = fwidth(crd);
    vec2 adjusted_cell_line_thickness    = 0.5 * ( cell_line_thickness    + d );
    vec2 adjusted_subcell_line_thickness = 0.5 * ( subcell_line_thickness + d );

    if ( distance_to_subcell.x < adjusted_subcell_line_thickness.x || distance_to_subcell.y < adjusted_subcell_line_thickness.y ) color = subcell_color;
    if ( distance_to_cell.x < adjusted_cell_line_thickness.x || distance_to_cell.y < adjusted_cell_line_thickness.y )    color = cell_color;

    /////////// ALPHA CLIPPING
    if (color.a < 0.1)
        discard;

    /////////// FADE
    float height_to_fade_distance_ratio = 30;
    float min_fade_distance = grid_size * 25;
    float max_fade_distance = grid_size * 50;

    float fade_distance = abs(viewPos.y) * height_to_fade_distance_ratio;

    fade_distance = max(fade_distance, min_fade_distance);
    fade_distance = min(fade_distance, max_fade_distance);

    float distance_to_camera = length(fragPos.xz - viewPos.xz);
    float opacity_falloff = smoothstep(1.0, 0.0, distance_to_camera / fade_distance);

    FragColor = color * opacity_falloff;
    //FragColor = vec4(1.0);
}