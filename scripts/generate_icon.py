#!/usr/bin/env python3
"""
Generate app icon for Resonant Graph Synth
A circular graph with glowing nodes and connections
"""

import math
from PIL import Image, ImageDraw, ImageFilter, ImageFont

def create_icon(size=1024):
    # Colors
    bg_color = (15, 23, 42)  # Dark blue-gray
    node_color = (168, 85, 247)  # Purple
    node_glow = (168, 85, 247, 100)
    line_color = (71, 85, 105, 150)  # Muted gray
    active_line = (168, 85, 247, 200)

    # Create image with alpha
    img = Image.new('RGBA', (size, size), (0, 0, 0, 0))
    draw = ImageDraw.Draw(img)

    # Background circle with gradient effect
    margin = size // 16
    bg_size = size - margin * 2

    # Draw background
    for i in range(3):
        offset = i * 2
        alpha = 255 - i * 30
        color = (bg_color[0], bg_color[1], bg_color[2], alpha)
        draw.ellipse([margin - offset, margin - offset,
                      margin + bg_size + offset, margin + bg_size + offset],
                     fill=color)

    # Graph parameters
    center = size // 2
    radius = size * 0.35
    num_nodes = 12
    node_radius = size // 20

    # Calculate node positions
    nodes = []
    for i in range(num_nodes):
        angle = -math.pi / 2 + (i / num_nodes) * 2 * math.pi
        x = center + radius * math.cos(angle)
        y = center + radius * math.sin(angle)
        nodes.append((x, y))

    # Draw connections (fifths pattern: 0-7, 1-8, 2-9, etc.)
    connections = [
        (0, 7), (7, 2), (2, 9), (9, 4), (4, 11), (11, 6),
        (6, 1), (1, 8), (8, 3), (3, 10), (10, 5), (5, 0),
        # Some thirds for visual interest
        (0, 4), (4, 7), (7, 11),
    ]

    # Draw dim connections first
    for i, (a, b) in enumerate(connections):
        x1, y1 = nodes[a]
        x2, y2 = nodes[b]
        # Vary line brightness
        alpha = 80 + (i % 3) * 40
        draw.line([(x1, y1), (x2, y2)],
                  fill=(100, 116, 139, alpha),
                  width=size // 200)

    # Draw active connections (brighter)
    active_connections = [(0, 7), (7, 2), (0, 4)]
    for a, b in active_connections:
        x1, y1 = nodes[a]
        x2, y2 = nodes[b]
        draw.line([(x1, y1), (x2, y2)],
                  fill=(168, 85, 247, 180),
                  width=size // 100)

    # Create glow layer
    glow_layer = Image.new('RGBA', (size, size), (0, 0, 0, 0))
    glow_draw = ImageDraw.Draw(glow_layer)

    # Draw node glows (some nodes "active")
    active_nodes = [0, 4, 7]  # C, E, G - a major chord

    for i, (x, y) in enumerate(nodes):
        # Glow size based on activity
        if i in active_nodes:
            glow_r = node_radius * 3
            glow_color = (168, 85, 247, 60)
        else:
            glow_r = node_radius * 1.5
            glow_color = (100, 116, 139, 30)

        glow_draw.ellipse([x - glow_r, y - glow_r, x + glow_r, y + glow_r],
                          fill=glow_color)

    # Blur the glow
    glow_layer = glow_layer.filter(ImageFilter.GaussianBlur(radius=size // 30))

    # Composite glow
    img = Image.alpha_composite(img, glow_layer)
    draw = ImageDraw.Draw(img)

    # Draw nodes
    for i, (x, y) in enumerate(nodes):
        # Node appearance based on activity
        if i in active_nodes:
            # Active node - bright purple
            node_fill = (168, 85, 247, 255)
            border_color = (255, 255, 255, 200)
            r = node_radius * 1.2
        else:
            # Inactive node - muted
            node_fill = (51, 65, 85, 255)
            border_color = (100, 116, 139, 150)
            r = node_radius

        draw.ellipse([x - r, y - r, x + r, y + r], fill=node_fill)
        draw.ellipse([x - r, y - r, x + r, y + r], outline=border_color, width=size // 200)

    # Add subtle center element (waveform hint)
    wave_r = size * 0.12
    for i in range(3):
        r = wave_r - i * (size * 0.03)
        alpha = 40 - i * 10
        draw.ellipse([center - r, center - r, center + r, center + r],
                     outline=(168, 85, 247, alpha), width=size // 150)

    return img

def create_iconset(base_path):
    """Create all required sizes for macOS iconset"""
    sizes = [16, 32, 64, 128, 256, 512, 1024]

    # Create high-res base
    base_icon = create_icon(1024)

    for s in sizes:
        # Regular
        icon = base_icon.resize((s, s), Image.Resampling.LANCZOS)
        icon.save(f"{base_path}/icon_{s}x{s}.png")

        # @2x for Retina (except 1024)
        if s <= 512:
            icon_2x = base_icon.resize((s * 2, s * 2), Image.Resampling.LANCZOS)
            icon_2x.save(f"{base_path}/icon_{s}x{s}@2x.png")

    print(f"Icons saved to {base_path}")

if __name__ == "__main__":
    import os
    import sys

    # Output directory
    out_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    icons_dir = os.path.join(out_dir, "resources", "icons")
    os.makedirs(icons_dir, exist_ok=True)

    # Generate icons
    create_iconset(icons_dir)

    # Also save a large PNG for reference
    icon = create_icon(1024)
    icon.save(os.path.join(icons_dir, "icon.png"))
    print(f"Main icon saved to {icons_dir}/icon.png")
