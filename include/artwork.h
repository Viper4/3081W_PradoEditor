#ifndef ARTWORK_H
#define ARTWORK_H
#include <string>
#include <opencv2/core.hpp>

struct Artwork
{
    // work_url,work_image_url,author,author_bio,author_url,author_id,work_title,work_subtitle,work_exposed,work_description,
    // work_tags,technical_sheet_numero_de_catalogo,technical_sheet_autor,technical_sheet_titulo,technical_sheet_fecha,technical_sheet_tecnica,
    // technical_sheet_soporte,technical_sheet_dimension,technical_sheet_serie,technical_sheet_procedencia,bibliography,inventory,expositions,
    // ubication,technical_sheet_autores,technical_sheet_edicion_/_estado,technical_sheet_materia,technical_sheet_ceca,technical_sheet_autora,
    // technical_sheet_lugar_de_produccion
    std::unordered_map<std::string, std::string> metadata;
};

enum SortCriteria {
    Title,
    Newest,
    Oldest,
    Artist
};

class PradoEditorMobileInterface
{
private:

public:
    std::vector<Artwork> artworkMapToVector(const std::unordered_map<std::string, Artwork>& map);
    void sortArtworks(const SortCriteria &criteria, std::vector<Artwork> &artworks);
};

#endif
