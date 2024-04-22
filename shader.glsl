#version 430 core
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout(std430, binding = 1) buffer BuffA
{
    float a[];
};
layout(std430, binding = 2) buffer BuffB
{
    float b[];
};
void main() {
    b[gl_WorkGroupID.x] += a[gl_WorkGroupID.x];
}
