#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform vec2 texelSize;

out vec4 finalColor;

void main()
{
    float center = texture(texture0, fragTexCoord).a;

    // Never draw over the champion itself
    if (center > 0.01) {
        finalColor = vec4(0.0);
        return;
    }

    float nearby = 0.0;

    for (int r = 1; r <= 3; r++) {
        vec2 o = texelSize * float(r);

        nearby = max(nearby, texture(texture0, fragTexCoord + vec2( o.x, 0.0)).a);
        nearby = max(nearby, texture(texture0, fragTexCoord + vec2(-o.x, 0.0)).a);
        nearby = max(nearby, texture(texture0, fragTexCoord + vec2(0.0,  o.y)).a);
        nearby = max(nearby, texture(texture0, fragTexCoord + vec2(0.0, -o.y)).a);

        nearby = max(nearby, texture(texture0, fragTexCoord + vec2( o.x,  o.y)).a);
        nearby = max(nearby, texture(texture0, fragTexCoord + vec2(-o.x,  o.y)).a);
        nearby = max(nearby, texture(texture0, fragTexCoord + vec2( o.x, -o.y)).a);
        nearby = max(nearby, texture(texture0, fragTexCoord + vec2(-o.x, -o.y)).a);
    }

    float outline = nearby * (1.0 - center);
    finalColor = vec4(1.0, 1.0, 1.0, outline);
}