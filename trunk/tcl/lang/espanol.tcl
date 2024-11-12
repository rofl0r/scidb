# ======================================================================
# Author : $Author$
# Version: $Revision: 166 $
# Date   : $Date: 2011-12-30 23:47:08 +0000 (Fri, 30 Dec 2011) $
# Url    : $URL$
# ======================================================================

# ======================================================================
#    _/|            __
#   // o\         /    )           ,        /    /
#   || ._)    ----\---------__----------__-/----/__-
#   //__\          \      /   '  /    /   /    /   )
#   )___(     _(____/____(___ __/____(___/____(___/_
# ======================================================================

# ======================================================================
# Copyright: (C) 2011 Gregor Cramer
# Copyright: (C) 2011 Carlos Fernando Gonzalez
# ======================================================================

# ======================================================================
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# ======================================================================

# ======================================================================
# File encoding: macRoman
# ======================================================================

### global #############################################################
::mc::SortMapping		{� a � A � e � E � i � I � n � N � o � O � u � U � u � U}
::mc::AsciiMapping	{� a � A � e � E � i � I � n � N � o � O � u � U � u � U}

::mc::Alignment		"Alineaci�n"
::mc::Apply				"Aplicar"
::mc::Background		"Fondo"
::mc::Black				"Negras"
::mc::Cancel			"Cancelar"
::mc::Clear				"Vaciar"
::mc::Close				"Cerrar"
::mc::Color				"Color"
::mc::Colors			"Colores"
::mc::Copy				"Copiar"
::mc::Cut				"Cortar"
::mc::Dark				"Oscuras"
::mc::Database			"Base"
::mc::Delete			"Eliminar"
::mc::Edit				"Editar"
::mc::Escape			"Esc"
::mc::From				"From" ;# NEW
::mc::Game				"Partida"
::mc::Game				"Partida"
::mc::Layout			"Disposici�n"
::mc::Lite				"Claras"
::mc::Modify			"Modificar"
::mc::No					"no"
::mc::NotAvailable	"n/d"
::mc::Number			"N�mero"
::mc::OK					"Aceptar"
::mc::Paste				"Pegar"
::mc::PieceSet			"Piezas"
::mc::Preview			"Vista previa"
::mc::Redo				"Deshacer"
::mc::Reset				"Restablecer"
::mc::SelectAll		"Seleccionar todo"
::mc::Texture			"Textura"
::mc::Theme				"Tema"
::mc::To					"To" ;# NEW
::mc::Undo				"Deshacer"
::mc::Variation		"Variante"
::mc::White				"Blancas"
::mc::Yes				"S�"

::mc::King				"Rey"
::mc::Queen				"Dama"
::mc::Rook				"Torre"
::mc::Bishop			"Alfil"
::mc::Knight			"Caballo"
::mc::Pawn				"Pe�n"

### locale #############################################################
::locale::Pattern(decimalPoint)	"."
::locale::Pattern(thousandsSep)	","
::locale::Pattern(dateY)			"Y"
::locale::Pattern(dateM)			"M Y"
::locale::Pattern(dateD)			"D M, Y"
::locale::Pattern(time)				"D M, Y, h:m"
::locale::Pattern(normal:dateY)	"Y"
::locale::Pattern(normal:dateM)	"M/Y"
::locale::Pattern(normal:dateD)	"D/M/Y"

### widget #############################################################
::widget::mc::Apply		"&Aplicar"
::widget::mc::Cancel		"&Cancelar"
::widget::mc::Clear		"&Vaciar"
::widget::mc::Close		"C&errar"
::widget::mc::Ok			"Acep&tar"
::widget::mc::Reset		"&Restablecer"
::widget::mc::Update		"Ac&tualizar"
::widget::mc::Import		"&Importar"
::widget::mc::Revert		"Re&vertir"
::widget::mc::Previous	"&Previo"
::widget::mc::Next		"Pr�&ximo"
::widget::mc::First		"Pr&imero" 
::widget::mc::Last		"�lti&mo" 

### util ###############################################################

::util::mc::IOErrorOccurred					"Hubo un error de I/O"

::util::mc::IOError(OpenFailed)				"apertura fallida"
::util::mc::IOError(ReadOnly)					"la base es de s�lo-lectura"
::util::mc::IOError(UnknownVersion)			"versi�n desconocida del archivo"
::util::mc::IOError(UnexpectedVersion)		"versi�n no esperada del archivo"
::util::mc::IOError(Corrupted)				"archivo corrupto"
::util::mc::IOError(WriteFailed)				"operaci�n de escritura fallida"
::util::mc::IOError(InvalidData)				"datos no v�lidos (archivo probablemente corrupto)" 
::util::mc::IOError(ReadError)				"error de lectura"
::util::mc::IOError(EncodingFailed)			"no se puede escribir el nombre" 
::util::mc::IOError(MaxFileSizeExceeded)	"se alcanz� el tama�o m�ximo de archivo" 
::util::mc::IOError(LoadFailed)				"carga fallida (demasiadas entradas de evento)" 

### progress ###########################################################
::progress::mc::Progress "Progreso"

### menu ###############################################################
::menu::mc::File						"&Archivo"
::menu::mc::Game						"&Partida"
::menu::mc::View						"&Vista"
::menu::mc::Help						"A&yuda"

::menu::mc::FileOpen					"A&brir"
::menu::mc::FileOpenURL				"Open &URL" ;# NEW
::menu::mc::FileOpenRecent			"Abrir &Recientes"
::menu::mc::FileNew					"&Nuevo"
::menu::mc::FileExport				"E&xportar..."
::menu::mc::FileImport				"Importar archivos P&GN..."
::menu::mc::FileImportOne			"&Importar una partida en PGN..."
::menu::mc::FileClose				"&Cerrar"
::menu::mc::FileQuit					"&Salir"

::menu::mc::GameNew					"N&ueva partida"
::menu::mc::GameNewChess960		"Nue&va partida: Ajedrez 960"
::menu::mc::GameNewChess960Sym	"Nuev&a partida: Ajedrez 960 (s�lo sim�trico)"
::menu::mc::GameNewShuffle			"Nu&eva partida: Ajedrez Shuffle"
::menu::mc::GameSave					"Guar&dar partida"
::menu::mc::GameReplace				"Ree&mplazar partida"
::menu::mc::GameReplaceMoves		"Reemp&lazar s�lo jugadas"

::menu::mc::HelpInfo					"&Informaci�n"
::menu::mc::HelpContents			"Con&tenidos"
::menu::mc::HelpBugReport			"Rep&orte de errores (abrir en navegador web)"
::menu::mc::HelpFeatureRequest	"&Solicitud de caracter�stica (abrir en navegador web)"

::menu::mc::ViewShowLog				"Mostrar bit�co&ra..."
::menu::mc::ViewFullscreen			"Pantalla completa" 

::menu::mc::OpenFile					"Abrir un archivo Scidb"
::menu::mc::NewFile					"Crear un archivo Scidb"
::menu::mc::ImportFiles				"Importar archivos PGN..."

::menu::mc::Theme						"Tema"
::menu::mc::Ctrl						"Ctrl"
::menu::mc::Shift						"Shift"

::menu::mc::AllScidbFiles			"Todos los archivos Scidb"
::menu::mc::AllScidbBases			"Todas las bases Scidb"
::menu::mc::ScidBases				"Bases Scid"
::menu::mc::ScidbBases				"Bases Scidb"
::menu::mc::ChessBaseBases			"Bases ChessBase"
::menu::mc::PGNFilesArchives		"PGN files/archives" ;# NEW
::menu::mc::PGNFiles					"Archivos PGN"
::menu::mc::PGNArchives				"PGN archives" ;# NEW

::menu::mc::FileNotAllowed			"Nombre de archivo '%s' no permitido"
::menu::mc::TwoOrMoreDots			"Contiene dos o m�s puntos consecutivos."
::menu::mc::ForbiddenChars			"Contiene caracteres prohibidos."

::menu::mc::Settings					"&Configuraci�n"

### load ###############################################################
::load::mc::FileIsCorrupt	"El archivo %s est� roto:"

::load::mc::Loading					"Cargando %s"
::load::mc::ReadingOptionsFile	"Leer archivo de opciones"
::load::mc::StartupFinished		"Inicio del programa completado"

::load::mc::ECOFile					"archivo ECO"
::load::mc::EngineFile				"archivo de motor"
::load::mc::SpellcheckFile			"archivo de revisi�n"
::load::mc::LocalizationFile		"archivo de localizaci�n"
::load::mc::RatingList				"listado de rating %s"
::load::mc::WikipediaLinks			"enlace a Wikipedia"
::load::mc::ChessgamesComLinks	"enlace a chessgames.com"
::load::mc::Cities					"ciudades"
::load::mc::PieceSet					"piezas"
::load::mc::Theme						"tema"
::load::mc::Icons						"�conos"

### application ########################################################
::application::mc::Database				"&Base"
::application::mc::Board					"&Tablero"

::application::mc::DockWindow				"Ventana acoplada"
::application::mc::UndockWindow			"Ventana desacoplada"
::application::mc::ChessInfoDatabase	"Base de Datos Ajedrec�stica"
::application::mc::Shutdown				"Cierre..."

### application::board #################################################
::application::board::mc::ShowCrosstable		"Mostrar grilla de torneo para esta partida"

::application::board::mc::Tools					"Herramientas"
::application::board::mc::Control				"Control"
::application::board::mc::GoIntoNextVar		"Ir a la pr�xima variante"
::application::board::mc::GoIntPrevVar			"Ir a la variante previa"

::application::board::mc::KeyEditAnnotation	"N"
::application::board::mc::KeyEditComment		"C"
::application::board::mc::KeyEditMarks			"M"

### application::database ##############################################
::application::database::mc::Games						"&Partidas"
::application::database::mc::Players					"&Jugadores"
::application::database::mc::Events						"Even&tos"
::application::database::mc::Annotators				"&Comentaristas"

::application::database::mc::File						"Archivo"
::application::database::mc::SymbolSize				"Tama�o del s�mbolo"
::application::database::mc::Large						"Grande"
::application::database::mc::Medium						"Mediano"
::application::database::mc::Small						"Peque�o"
::application::database::mc::Tiny						"Diminuto"
::application::database::mc::Empty						"vac�o"
::application::database::mc::None						"ninguno"
::application::database::mc::Failed						"fallido"
::application::database::mc::LoadMessage				"Abrir Base %s"
::application::database::mc::UpgradeMessage			"Upgrading Database %s" ;# NEW
::application::database::mc::CannotOpenFile			"No se puede abrir el archivo '%s'."
::application::database::mc::EncodingFailed			"Fallo en la codificaci�n de %s."
::application::database::mc::DatabaseAlreadyOpen	"La Base '%s' ya est� abierta."
::application::database::mc::Properties				"Propiedades"
::application::database::mc::Preload					"Precarga"
::application::database::mc::MissingEncoding			"Codificaci�n %s perdida (usar %s en su lugar)"
::application::database::mc::DescriptionTooLarge	"Descripci�n demasiado grande."
::application::database::mc::DescrTooLargeDetail	"La entrada contiene %d caracteres, pero s�lo se permiten %d."
::application::database::mc::ClipbaseDescription	"Base temporal, no se guarda al disco."
::application::database::mc::HardLinkDetected		"Cannot load file '%file1' because it is already loaded as file '%file2'. This can only happen if hard links are involved." ;# NEW
::application::database::mc::HardLinkDetectedDetail "If we load this database twice the application may crash due to the usage of threads." ;# NEW

::application::database::mc::RecodingDatabase		"Recodificar %base de %from a %to"
::application::database::mc::RecodedGames				"%s partida(s) recodificadas"

::application::database::mc::GameCount					"Partidas"
::application::database::mc::DatabasePath				"ruta a la Base"
::application::database::mc::DeletedGames				"Partidas eliminadas"
::application::database::mc::Description				"Descripci�n"
::application::database::mc::Created					"Creada"
::application::database::mc::LastModified				"�ltima modificaci�n"
::application::database::mc::Encoding					"Codificar"
::application::database::mc::YearRange					"Rango de a�os"
::application::database::mc::RatingRange				"Rango de ratings"
::application::database::mc::Result						"Resultado"
::application::database::mc::Score						"puntuaci�n"
::application::database::mc::Type						"Tipo"
::application::database::mc::ReadOnly					"S�lo lectura"

::application::database::mc::ChangeIcon				"Cambiar �cono"
::application::database::mc::Recode						"Recodificar"
::application::database::mc::EditDescription			"Editar Descripci�n"
::application::database::mc::EmptyClipbase			"vaciar Base temporal"

::application::database::mc::T_Unspecific				"Inespec�fico"
::application::database::mc::T_Temporary				"Temporario"
::application::database::mc::T_Work						"Trabajo"
::application::database::mc::T_Clipbase				"Base temporal"
::application::database::mc::T_MyGames					"Mis partidas"
::application::database::mc::T_Informant				"Informador"
::application::database::mc::T_LargeDatabase			"Gran Base"
::application::database::mc::T_CorrespondenceChess	"Ajedrez por Correspondencia"  
::application::database::mc::T_EmailChess				"Ajedrez por email"
::application::database::mc::T_InternetChess			"Ajedrez por Internet"
::application::database::mc::T_ComputerChess			"Ajedrez por computadora"
::application::database::mc::T_Chess960				"Ajedrez 960"
::application::database::mc::T_PlayerCollection		"Colecci�n de jugadores"
::application::database::mc::T_Tournament				"Torneo"
::application::database::mc::T_TournamentSwiss		"Torneo suizo"
::application::database::mc::T_GMGames					"Partidas de GM"
::application::database::mc::T_IMGames					"Partidas de IM"
::application::database::mc::T_BlitzGames				"Partidas r�pidas"
::application::database::mc::T_Tactics					"T�ctica"
::application::database::mc::T_Endgames				"Finales"
::application::database::mc::T_Analysis				"An�lisis"
::application::database::mc::T_Training				"Entrenamiento"
::application::database::mc::T_Match					"Competencia"
::application::database::mc::T_Studies					"Estudios"
::application::database::mc::T_Jewels					"Joyas"
::application::database::mc::T_Problems				"Problemas"
::application::database::mc::T_Patzer					"Novato"
::application::database::mc::T_Gambit					"Gambito"
::application::database::mc::T_Important				"Importante"
::application::database::mc::T_Openings				"Aperturas"
::application::database::mc::T_OpeningsWhite			"Aperturas de las Blancas"
::application::database::mc::T_OpeningsBlack			"Aperturas de las Negras"

::application::database::mc::OpenDatabase				"Abrir Base"
::application::database::mc::NewDatabase				"Nueva Base"
::application::database::mc::CloseDatabase			"Cerrar Base '%s'"
::application::database::mc::SetReadonly				"Set Database '%s' readonly" ;# NEW
::application::database::mc::SetWriteable				"Set Database '%s' writeable" ;# NEW

::application::database::mc::OpenReadonly				"Abrir en solo-lectura"
::application::database::mc::OpenWriteable			"Abrir con permiso de escritura"

::application::database::mc::UpgradeDatabase			"%s is an old format database that cannot be opened writeable.\n\nUpgrading will create a new version of the database and after that remove the original files.\n\nThis may take a while, but it only needs to be done one time.\n\nDo you want to upgrade this database now?" ;# NEW
::application::database::mc::UpgradeDatabaseDetail	"\"No\" will open the database readonly, and you cannot set it writeable." ;# NEW

### application::database::games #######################################
::application::database::games::mc::Control						"Control"
::application::database::games::mc::GameNumber					"N�mero de partida"

::application::database::games::mc::GotoFirstPage				"Ir a la primera p�gina de partidas"
::application::database::games::mc::GotoLastPage				"Ir a la �ltima p�gina de partidas"
::application::database::games::mc::PreviousPage				"P�gina de partidas previa"
::application::database::games::mc::NextPage						"Pr�xima p�gina de partidas"
::application::database::games::mc::GotoCurrentSelection		"Ir a la selecci�n actual"
::application::database::games::mc::UseVerticalScrollbar		"Usar barra de deslizamiento vertical"
::application::database::games::mc::UseHorizontalScrollbar	"Usar barra de deslizamiento horizontal"
::application::database::games::mc::GotoEnteredGameNumber	"Ir al n�mero de partida ingresado"

### application::database::players #####################################
::application::database::players::mc::EditPlayer				"Editar Jugador"
::application::database::players::mc::Score						"Puntuaci�n"
::application::database::players::mc::TooltipRating			"Rating: %s"

### application::database::annotators ##################################
::application::database::annotators::mc::F_Annotator		"Comentarista"
::application::database::annotators::mc::F_Frequency		"Frecuencia"

::application::database::annotators::mc::Find				"Buscar"
::application::database::annotators::mc::FindAnnotator	"Buscar comentarista"
::application::database::annotators::mc::ClearEntries		"Vaciar entradas"
::application::database::annotators::mc::NotFound			"No se encontr�."

### application::pgn ###################################################
::application::pgn::mc::Command(move:comment)			"Agregar comentario"
::application::pgn::mc::Command(move:marks)				"Agregar marcador"
::application::pgn::mc::Command(move:annotation)		"Agregar Nota/Comentario/Marcador"
::application::pgn::mc::Command(move:append)				"Agregar jugada"
::application::pgn::mc::Command(move:nappend)			"Add Moves" ;# NEW
::application::pgn::mc::Command(move:exchange)			"Cambiar jugada"
::application::pgn::mc::Command(variation:new)			"Agregar variante"
::application::pgn::mc::Command(variation:replace)		"Reemplazar jugadas"
::application::pgn::mc::Command(variation:truncate)	"Truncar variante"
::application::pgn::mc::Command(variation:first)		"Convertir en primera variante"
::application::pgn::mc::Command(variation:promote)		"Transformar variante en L�nea principal"
::application::pgn::mc::Command(variation:remove)		"Eliminar variante"
::application::pgn::mc::Command(variation:mainline)	"Nueva L�nea principal"
::application::pgn::mc::Command(variation:insert)		"Agregar jugadas"
::application::pgn::mc::Command(variation:exchange)	"Permutar jugadas"
::application::pgn::mc::Command(strip:moves)				"Jugadas desde el principio"
::application::pgn::mc::Command(strip:truncate)			"Jugadas hasta el final"
::application::pgn::mc::Command(strip:annotations)		"Notas"
::application::pgn::mc::Command(strip:info)				"Move Information" ;# NEW
::application::pgn::mc::Command(strip:marks)				"Marcadores"
::application::pgn::mc::Command(strip:comments)			"Comentarios"
::application::pgn::mc::Command(strip:variations)		"Variantes"
::application::pgn::mc::Command(copy:comments)			"Copy Comments" ;# NEW
::application::pgn::mc::Command(move:comments)			"Move Comments" ;# NEW
::application::pgn::mc::Command(game:clear)				"Vaciar partida"
::application::pgn::mc::Command(game:transpose)			"Partida transpuesta"

::application::pgn::mc::StartTrialMode						"Iniciar el modo de prueba"
::application::pgn::mc::StopTrialMode						"Terminar el modo de prueba"
::application::pgn::mc::Strip									"Limpiar"
::application::pgn::mc::InsertDiagram						"Insertar diagrama"
::application::pgn::mc::InsertDiagramFromBlack			"Insertar diagrama desde la perspectiva de las Negras"
::application::pgn::mc::SuffixCommentaries				"Comentarios en los sufijos"
::application::pgn::mc::StripOriginalComments			"Strip original comments" ;# NEW

::application::pgn::mc::AddNewGame							"Guardar: Agregar nueva partida a %s..."
::application::pgn::mc::ReplaceGame							"Guardar: Reemplazar partida en %s..."
::application::pgn::mc::ReplaceMoves						"Guardar: Reemplazar jugadas s�lo en la partida"

::application::pgn::mc::ColumnStyle							"Estilo columna"
::application::pgn::mc::UseParagraphSpacing				"Usar espaciado en los p�rrafos"
::application::pgn::mc::ShowMoveInfo						"Show Move Information" ;# NEW
::application::pgn::mc::BoldTextForMainlineMoves		"Negrita para las jugadas de la L�nea principal"
::application::pgn::mc::ShowDiagrams						"Mostrar diagramas"
::application::pgn::mc::Languages							"Idiomas"
::application::pgn::mc::CollapseVariations				"Contraer variantes"
::application::pgn::mc::ExpandVariations					"Expandir variantes"
::application::pgn::mc::EmptyGame							"Vaciar partida"

::application::pgn::mc::NumberOfMoves						"N�mero de medias jugadas (en la l�nea principal):"
::application::pgn::mc::InvalidInput						"Entrada no v�lida '%d'."
::application::pgn::mc::MustBeEven							"La entrada debe ser par."
::application::pgn::mc::MustBeOdd							"La entrada debe ser impar."
::application::pgn::mc::ReplaceMovesSucceeded			"Jugadas reemplazadas exitosamente."
::application::pgn::mc::CannotOpenCursorFiles			"Cannot open cursor files: %s" ;# NEW

::application::pgn::mc::EditAnnotation						"Editar nota"
::application::pgn::mc::EditMoveInformation				"Edit move information" ;# NEW
::application::pgn::mc::EditCommentBefore					"Editar comentario antes de la jugada"
::application::pgn::mc::EditCommentAfter					"Editar comentario tras la jugada"
::application::pgn::mc::EditPrecedingComment				"Editar el comentario precedente"
::application::pgn::mc::EditTrailingComment				"Editar �ltimo comentario" 
::application::pgn::mc::EditMarks							"Editar marcador"
::application::pgn::mc::Display								"Mostrar"
::application::pgn::mc::None									"ninguno"

### application::tree ##################################################

::application::tree::mc::Total								"Total"
::application::tree::mc::Control								"Control"
::application::tree::mc::ChooseReferenceBase				"Elegir base de referencia"
::application::tree::mc::ReferenceBaseSwitcher			"Selector de base de referencia"
::application::tree::mc::Numeric								"Num�rico"
::application::tree::mc::Bar									"Barras"
::application::tree::mc::StartSearch						"Iniciar b�squeda"
::application::tree::mc::StopSearch							"Suspender b�squeda"
::application::tree::mc::UseExactMode						"Usar posici�n de b�squeda"
::application::tree::mc::UseFastMode						"Usar b�squeda acelerada"
::application::tree::mc::UseQuickMode						"Usar b�squeda r�pida"
::application::tree::mc::AutomaticSearch					"Busqueda autom�tica"
::application::tree::mc::LockReferenceBase				"Bloquear base de referencia"
::application::tree::mc::TransparentBar					"Barras transparentes"

::application::tree::mc::FromWhitesPerspective			"Desde el lado de las Blancas"
::application::tree::mc::FromBlacksPerspective			"Desde el lado de las Negras"
::application::tree::mc::FromSideToMovePerspective		"Desde el lado que mueve"
::application::tree::mc::FromWhitesPerspectiveTip		"Score from whites perspective" ;# NEW
::application::tree::mc::FromBlacksPerspectiveTip		"Score from blacks perspective" ;# NEW

::application::tree::mc::TooltipAverageRating			"Rating promedio (%s)"
::application::tree::mc::TooltipBestRating				"Mejor rating (%s)"

::application::tree::mc::F_Number							"#"
::application::tree::mc::F_Move								"Jugada"
::application::tree::mc::F_Eco								"ECO"
::application::tree::mc::F_Frequency						"Frecuencia"
::application::tree::mc::F_Ratio								"Proporci�n"
::application::tree::mc::F_Score								"Resultado"
::application::tree::mc::F_Draws								"Tablas"
::application::tree::mc::F_Performance						"Desempe�o"
::application::tree::mc::F_AverageYear						"\u00f8 A�o"
::application::tree::mc::F_LastYear							"�ltima partida jugada"
::application::tree::mc::F_BestPlayer						"Mejor jugador"
::application::tree::mc::F_FrequentPlayer					"Jugador m�s frecuente"

::application::tree::mc::T_Number							"Numeraci�n"
::application::tree::mc::T_AverageYear						"A�o promedio"
::application::tree::mc::T_FrequentPlayer					"Jugador m�s frecuente"

### board ##############################################################
::board::mc::CannotReadFile		"No se puede leer el archivo '%s':"
::board::mc::CannotFindFile		"No se encuentra el archivo '%s'"
::board::mc::FileWillBeIgnored	"Se ignorar� '%s' (ID duplicado)"
::board::mc::IsCorrupt				"'%s' est� da�ado (estilo %s desconocido '%s')"

::board::mc::ThemeManagement		"Manejo de temas"
::board::mc::Setup					"Disposici�n"

::board::mc::Default					"Por defecto"
::board::mc::WorkingSet				"Conjunto usado"

### board::options #####################################################
::board::options::mc::Coordinates			"Coordenadas"
::board::options::mc::SolidColor				"Color s�lido"
::board::options::mc::EditList				"Editar lista"
::board::options::mc::Embossed				"repujado"
::board::options::mc::Highlighting			"Resaltar"
::board::options::mc::Border					"Borde"
::board::options::mc::SaveWorkingSet		"Guardar el conjunto usado"
::board::options::mc::SelectedSquare		"Casilla elegida"
::board::options::mc::ShowBorder				"Mostrar borde"
::board::options::mc::ShowCoordinates		"Mostrar coordenadas"
::board::options::mc::ShowMaterialValues	"Mostrar material"
::board::options::mc::ShowBar					"Mostrar barras"
::board::options::mc::ShowSideToMove		"Mostrar el lado que mueve"
::board::options::mc::ShowSuggestedMove	"Mostar jugada sugerida"
::board::options::mc::SuggestedMove			"Jugada sugerida"
::board::options::mc::Basic					"B�sico"
::board::options::mc::PieceStyle				"Estilo de pieza"
::board::options::mc::SquareStyle			"Estilo de casilla"
::board::options::mc::Styles					"Estilos"
::board::options::mc::Show						"Vista previa"
::board::options::mc::ChangeWorkingSet		"Editar conjunto usado"
::board::options::mc::CopyToWorkingSet		"Copiar al conjunto usado"
::board::options::mc::NameOfPieceStyle		"Ingresar nombre del estilo de piezas"
::board::options::mc::NameOfSquareStyle	"Ingresar nombre de estilo de casillas"
::board::options::mc::NameOfThemeStyle		"Ingresar nombre del tema"
::board::options::mc::PieceStyleSaved		"Estilo de pieza '%s' guardado como '%s'"
::board::options::mc::SquareStyleSaved		"Estilo de casillas '%s' guardado como '%s'"
::board::options::mc::ChooseColors			"Elija colores"
::board::options::mc::SupersedeSuggestion	"Cambiar/usar colores sugeridos en estilo de casillas"
::board::options::mc::CannotDelete			"No se puede eliminar '%s'."
::board::options::mc::IsWriteProtected		"El archivo '%s' est� protegido contra escritura."
::board::options::mc::ConfirmDelete			"�Est� seguro de eliminar '%s'?"
::board::options::mc::NoPermission			"No se puede eliminar '%s'.\nPermiso denegado."
::board::options::mc::BoardSetup				"Disposici�n del tablero"
::board::options::mc::OpenTextureDialog	"Abrir di�logo de texturas"

::board::options::mc::YouCannotReverse
	"No puede revertir esta acci�n. El archivo '%s' ser� removido f�sicamente."

::board::options::mc::CannotUsePieceWorkingSet
	"No se puede crear un nuevo tema con el estilo de piezas %s elegido.\n Primero debe guardar el nuevo estilo de pieza, o elegir otro."

::board::options::mc::CannotUseSquareWorkingSet
	"No se puede crear un nuevo tema con el estilo de casillas %s elegido.\n Primero debe guardar el nuevo estilo de casillas, o elegir otro."

### board::piece #######################################################
::board::piece::mc::Start						"Iniciar"
::board::piece::mc::Stop						"Parar"
::board::piece::mc::HorzOffset				"Impresi�n horizontal"
::board::piece::mc::VertOffset				"Impresi�n vertical"
::board::piece::mc::Gradient					"Gradiente"
::board::piece::mc::Fill						"Relleno"
::board::piece::mc::Stroke						"Trazo"
::board::piece::mc::Contour					"Contorno"
::board::piece::mc::WhiteShape				"Silueta blanca"
::board::piece::mc::PieceSelection			"Selecci�n de piezas"
::board::piece::mc::BackgroundSelection	"Selecci�n de fondo"
::board::piece::mc::Zoom						"Zoom"
::board::piece::mc::Shadow						"Sombra"
::board::piece::mc::Opacity					"Opacidad"
::board::piece::mc::ShadowDiffusion			"Difusi�n de la sombra"
::board::piece::mc::PieceStyleConf			"Configuraci�n del estilo de pieza"
::board::piece::mc::Offset						"Impresi�n"
::board::piece::mc::Rotate						"Rotar"
::board::piece::mc::CloseDialog				"�Cerrar el di�logo y descartar cambios?"
::board::piece::mc::OpenTextureDialog		"Abrir di�logo de textura"

### board::square ######################################################
::board::square::mc::SolidColor			"Color s�lido"
::board::square::mc::CannotReadFile		"No se puede leer el archivo"
::board::square::mc::Zoom					"Zoom"
::board::square::mc::Offset				"Imprimir"
::board::square::mc::Rotate				"Rotar"
::board::square::mc::Borderline			"Borde"
::board::square::mc::Width					"Ancho"
::board::square::mc::Opacity				"Opacidad"
::board::square::mc::GapBetweenSquares	"Espacio entre casillas"
::board::square::mc::Highlighting		"Resaltar"
::board::square::mc::Selected				"Elegido"
::board::square::mc::SuggestedMove		"Jugada sugerida"
::board::square::mc::Show					"Vista previa"
::board::square::mc::SquareStyleConf	"Configuraci�n del estilo de las casillas"
::board::square::mc::CloseDialog			"�Cerrar el di�logo y descartar los cambios?"

### board::texture #####################################################
::board::texture::mc::PreselectedOnly "S�lo preseleccionados"

### gametable ##########################################################
::gametable::mc::DeleteGame				"Marcar partida como eliminada"
::gametable::mc::UndeleteGame				"Recuperar esta partida"
::gametable::mc::EditGameFlags			"Editar insignias de la partida"
::gametable::mc::Custom						"Habitual"

::gametable::mc::Monochrome				"Monocromo"
::gametable::mc::Transparent				"Transparente"
::gametable::mc::Relief						"Relieve"
::gametable::mc::ShowIdn					"Mostrar n�mero de posici�n en Chess 960"
::gametable::mc::Icons						"Iconos"
::gametable::mc::Abbreviations			"Abreviaturas"

::gametable::mc::SortAscending			"Clasificar (ascendente)"
::gametable::mc::SortDescending			"Clasificar (descendente)"
::gametable::mc::SortOnAverageElo		"Clasificar por Elo promedio (descendente)"
::gametable::mc::SortOnAverageRating	"Clasificar por rating promedio (descendente)"
::gametable::mc::SortOnDate				"Clasificar por fecha (descendente)"
::gametable::mc::SortOnNumber				"Clasificar por n�mero de partida (ascendente)"
::gametable::mc::ReverseOrder				"Invertir el orden"
::gametable::mc::NotAvailable				"n/d"
::gametable::mc::NoMoves					"Sin jugadas"
::gametable::mc::NoMoreMoves				"No more moves" ;# NEW
::gametable::mc::WhiteRating				"Rating de las Blancas"
::gametable::mc::BlackRating				"Rating de las Negras"

::gametable::mc::Flags						"Insignias"
::gametable::mc::PGN_CountryCode			"C�digo PGN de pa�s"
::gametable::mc::ISO_CountryCode			"C�digo ISO de pa�s"
::gametable::mc::ExcludeElo				"Excluir Elo"
::gametable::mc::IncludePlayerType		"Incluir tipo de jugador"
::gametable::mc::ShowTournamentTable	"Grilla del torneo"

::gametable::mc::Long						"Largo"
::gametable::mc::Short						"Corto"

::gametable::mc::AccelBrowse				"W"
::gametable::mc::AccelOverview			"O"
::gametable::mc::AccelTournTable			"T"
::gametable::mc::Space						"Espacio"

::gametable::mc::F_Number					"#"
::gametable::mc::F_White					"Blancas"
::gametable::mc::F_Black					"Negras"
::gametable::mc::F_Event					"Evento"
::gametable::mc::F_Site						"Lugar"
::gametable::mc::F_Date						"Fecha"
::gametable::mc::F_Result					"Resultado"
::gametable::mc::F_Round					"Ronda"
::gametable::mc::F_Annotator				"Comentarista"
::gametable::mc::F_Length					"Longitud"
::gametable::mc::F_Termination			"Terminaci�n"
::gametable::mc::F_EventMode				"Modo"
::gametable::mc::F_Eco						"ECO"
::gametable::mc::F_Flags					"Insignias"
::gametable::mc::F_Material				"Material"
::gametable::mc::F_Acv						"ACV"
::gametable::mc::F_Idn						"960"
::gametable::mc::F_Position				"Posici�n"
::gametable::mc::F_EventDate				"Fecha del Evento"
::gametable::mc::F_EventType				"Tipo de Ev."
::gametable::mc::F_Changed					"Modificado"
::gametable::mc::F_Promotion				"Promoci�n"
::gametable::mc::F_UnderPromo				"Sub-promoci�n"
::gametable::mc::F_StandardPos			"Posici�n est�ndar"
::gametable::mc::F_Chess960Pos			"9"
::gametable::mc::F_Opening					"Apertura"
::gametable::mc::F_Variation				"Variante"
::gametable::mc::F_Subvariation			"Subvariante"
::gametable::mc::F_Overview				"Visi�n general"
::gametable::mc::F_Key						"C�digo ECO interno"

::gametable::mc::T_Number					"N�mero"
::gametable::mc::T_Acv						"Notas / Comentarios / Variantes"
::gametable::mc::T_WhiteRatingType		"Tipo de valuaci�n de las Blancas"
::gametable::mc::T_BlackRatingType		"Tipo de valuaci�n de las Negras"
::gametable::mc::T_WhiteCountry			"Federaci�n de las Blancas"
::gametable::mc::T_BlackCountry			"Federaci�n de las Negras"
::gametable::mc::T_WhiteTitle				"T�tulo de las Blancas"
::gametable::mc::T_BlackTitle				"T�tulo de las Negras"
::gametable::mc::T_WhiteType				"Tipo de las Blancas"
::gametable::mc::T_BlackType				"Tipo de las Negras"
::gametable::mc::T_WhiteSex				"Sexo de las Blancas"
::gametable::mc::T_BlackSex				"Sexo de las Negras"
::gametable::mc::T_EventCountry			"Pa�s del Evento"
::gametable::mc::T_EventType				"Tipo de Evento"
::gametable::mc::T_Chess960Pos			"Posici�n en Chess 960"
::gametable::mc::T_Deleted					"Eliminado"
::gametable::mc::T_EngFlag					"Insignia de idioma ingl�s"
::gametable::mc::T_OthFlag					"Insignia de otro idioma"
::gametable::mc::T_Idn						"N�mero de posici�n en Chess 960"
::gametable::mc::T_Annotations			"Notas"
::gametable::mc::T_Comments				"Comentarios"
::gametable::mc::T_Variations				"Variantes"
::gametable::mc::T_TimeMode				"Control de tiempo"

::gametable::mc::P_Rating					"Puntaje de rating"
::gametable::mc::P_RatingType				"Tipo de rating"
::gametable::mc::P_Country					"Pa�s"
::gametable::mc::P_Title					"T�tulo"
::gametable::mc::P_Type						"Tipo"
::gametable::mc::P_Date						"Fecha"
::gametable::mc::P_Mode						"Modo"
::gametable::mc::P_Sex						"Sexo"
::gametable::mc::P_Name						"Nombre"

::gametable::mc::G_White					"Blancas"
::gametable::mc::G_Black					"Negras"
::gametable::mc::G_Event					"Evento"

::gametable::mc::EventType(game)			"Partida"
::gametable::mc::EventType(match)		"Match"
::gametable::mc::EventType(tourn)		"Torneo"
::gametable::mc::EventType(swiss)		"Suizo"
::gametable::mc::EventType(team)			"Equipo"
::gametable::mc::EventType(k.o.)			"K.O."
::gametable::mc::EventType(simul)		"Simult�neas"
::gametable::mc::EventType(schev)		"Scheveningen"

::gametable::mc::PlayerType(human)		"Humano"
::gametable::mc::PlayerType(program)	"Computadora"

::gametable::mc::GameFlags(w)				"Apertura de las Blancas"
::gametable::mc::GameFlags(b)				"Apertura de las Negras"
::gametable::mc::GameFlags(m)				"Medio juego"
::gametable::mc::GameFlags(e)				"Final"
::gametable::mc::GameFlags(N)				"Novedad"
::gametable::mc::GameFlags(p)				"Estructura de peones"
::gametable::mc::GameFlags(T)				"T�ctica"
::gametable::mc::GameFlags(K)				"Ala de rey"
::gametable::mc::GameFlags(Q)				"Ala de dama"
::gametable::mc::GameFlags(!)				"Genialidad"
::gametable::mc::GameFlags(?)				"Metida de pata"
::gametable::mc::GameFlags(U)				"Usuario"
::gametable::mc::GameFlags(*)				"Mejor partida"
::gametable::mc::GameFlags(D)				"Torneo resuelto"
::gametable::mc::GameFlags(G)				"Partida modelo"
::gametable::mc::GameFlags(S)				"Estrategia"
::gametable::mc::GameFlags(^)				"Ataque"
::gametable::mc::GameFlags(~)				"Sacrificio"
::gametable::mc::GameFlags(=)				"Defensa"
::gametable::mc::GameFlags(M)				"Material"
::gametable::mc::GameFlags(P)				"Juego de piezas"
::gametable::mc::GameFlags(t)				"Horror t�ctico" ; # unused
::gametable::mc::GameFlags(s)				"Horror estrat�gico" ; # unused
::gametable::mc::GameFlags(C)				"Illegal Castling" ;# NEW
::gametable::mc::GameFlags(I)				"Jugada ilegal"

### playertable ########################################################
::playertable::mc::F_LastName					"Apellido"
::playertable::mc::F_FirstName				"Nombre"
::playertable::mc::F_FideID					"Fide ID"
::playertable::mc::F_Title						"T�tulo"
::playertable::mc::F_Frequency				"Frecuencia"

::playertable::mc::T_Federation				"Federaci�n"
::playertable::mc::T_RatingType				"Tipo de rating"
::playertable::mc::T_Type						"Tipo"
::playertable::mc::T_Sex						"Sexo"
::playertable::mc::T_PlayerInfo				"Bandera"

::playertable::mc::Find							"B�squeda"
::playertable::mc::StartSearch				"Iniciar b�squeda"
::playertable::mc::ClearEntries				"Vaciar entradas"
::playertable::mc::NotFound					"No se encontr�."

::playertable::mc::Name							"Nombre"
::playertable::mc::HighestRating				"Mayor rating"
::playertable::mc::MostRecentRating			"Rating m�s reciente"
::playertable::mc::DateOfBirth				"Fecha de nacimiento"
::playertable::mc::DateOfDeath				"Fecha de fallecimiento"
::playertable::mc::FideID						"Fide ID"

::playertable::mc::OpenInWebBrowser			"Abrir en un navegador..."
::playertable::mc::OpenPlayerCard			"Abrir la tarjeta del jugador %s"
::playertable::mc::OpenFileCard				"Abrir la tarjeta del archivo %s"
::playertable::mc::OpenFideRatingHistory	"Abrir el historial de ratings de la Fide"
::playertable::mc::OpenWikipedia				"Abrir la biograf�a de Wikipedia"
::playertable::mc::OpenViafCatalog			"Abrir el cat�logo VIAF"
::playertable::mc::OpenPndCatalog			"Abrir el cat�logo de la Deutsche Nationalbibliothek"
::playertable::mc::OpenChessgames			"Colecci�n de partidas de chessgames.com"
::playertable::mc::SeachIn365ChessCom		"Search in 365Chess.com" ;# NEW

### eventtable #########################################################
::eventtable::mc::Attendance	"Attendance" ;# NEW

### gamebar ############################################################
::gamebar::mc::StartPosition			"Iniciar posici�n"
::gamebar::mc::Players					"Jugadores"
::gamebar::mc::Event						"Evento"
::gamebar::mc::Site						"Lugar"
::gamebar::mc::SeparateHeader			"Encabezado separado"
::gamebar::mc::ShowActiveAtBottom	"Mostrar la partida activa al pie"
::gamebar::mc::ShowPlayersOnSeparateLines	"Mostrar los jugadores en l�neas separadas"
::gamebar::mc::DiscardChanges			"Esta partida ha cambiado.\n\n�Quiere realmente descartar los cambios realizados?"
::gamebar::mc::DiscardNewGame			"�Realmente quiere descartar esta partida?"
::gamebar::mc::NewGameFstPart			"Nueva"
::gamebar::mc::NewGameSndPart			"Partida"
::gamebar::mc::Unlock					"Unlock" ;# NEW

::gamebar::mc::LockGame					"Lock Game" ;# NEW
::gamebar::mc::CloseGame				"Close Game" ;# NEW

### browser ############################################################
::browser::mc::BrowseGame			"Buscar partida"
::browser::mc::StartAutoplay		"Comenzar Autojuego"
::browser::mc::StopAutoplay		"Parar Autojuego"
::browser::mc::GoForward			"Avanzar una jugada"
::browser::mc::GoBackward			"Retroceder una jugada"
::browser::mc::GoForwardFast		"Avanzar algunas jugadas"
::browser::mc::GoBackFast			"Retroceder algunas jugadas"
::browser::mc::GotoStartOfGame	"Ir al inicio de la partida"
::browser::mc::GotoEndOfGame		"Ir al final de la partida"
::browser::mc::IncreaseBoardSize	"Agrandar el tablero"
::browser::mc::DecreaseBoardSize	"Achicar el tablero"
::browser::mc::GotoFirstGame		"Ir a la primera partida"
::browser::mc::GotoLastGame		"Ir a la �ltima partida"

::browser::mc::LoadGame				"Cargar partida"
::browser::mc::MergeGame			"Fusionar partida"

::browser::mc::IllegalMove			"Jugada ilegal"
::browser::mc::NoCastlingRights	"no puede enrocar"

### overview ###########################################################
::overview::mc::Overview				"Pantallazo"
::overview::mc::RotateBoard			"Girar el tablero"
::overview::mc::AcceleratorRotate	"R"

### encoding ###########################################################
::encoding::mc::AutoDetect				"auto-detection" ;# NEW

::encoding::mc::Encoding				"Codificar"
::encoding::mc::Description			"Descripci�n"
::encoding::mc::Languages				"Idiomas (Fuentes)"
::encoding::mc::UseAutoDetection		"Use Auto-Detection" ;# NEW

::encoding::mc::ChooseEncodingTitle	"Elegir C�digo"

::encoding::mc::CurrentEncoding		"Codificaci�n actual:"
::encoding::mc::DefaultEncoding		"Codificaci�n predeterminada:"
::encoding::mc::SystemEncoding		"Codificaci�n del sistema:"

### setup ##############################################################
::setup::board::mc::Chess960Position		"Posici�n de Chess 960"
::setup::board::mc::SymmChess960Position	"Posici�n sim�trica de chess 960"
::setup::board::mc::ShuffleChessPosition	"Posici�n de ajedrez Shuffle"

### setup board ########################################################
::setup::position::mc::SetStartPosition		"Configurar una posici�n inicial"
::setup::position::mc::UsePreviousPosition	"Usar una posici�n previa"

::setup::board::mc::SetStartBoard				"Configurar tablero de inicio"
::setup::board::mc::SideToMove					"Lado que mueve"
::setup::board::mc::Castling						"Enroque"
::setup::board::mc::MoveNumber					"N�mero de jugada"
::setup::board::mc::EnPassantFile				"Al paso"
::setup::board::mc::StartPosition				"Posici�n inicial"
::setup::board::mc::Fen								"FEN"
::setup::board::mc::Clear							"Vaciar"
::setup::board::mc::CopyFen						"Copiar FEN al portapapeles"
::setup::board::mc::Shuffle						"Shuffle..."
::setup::board::mc::StandardPosition			"Posici�n est�ndar"
::setup::board::mc::Chess960Castling			"Enroque en Chess 960"

::setup::board::mc::InvalidFen					"FEN no v�lido"
::setup::board::mc::CastlingWithoutRook		"Se han fijado derechos de enroque pero al menos una de las torres no est�. Esto s�lo sucede en  This can happen only in partidas con ventaja. �Est� seguro que los derechos de enroque son correctos?"
::setup::board::mc::UnsupportedVariant			"La posici�n es una posici�n de inicio pero no corresponde a una posici�n de Shuffle Chess. �Est� seguro?"

::setup::board::mc::Error(InvalidFen)					"FEN no v�lido."
::setup::board::mc::Error(NoWhiteKing)					"Sin rey blanco."
::setup::board::mc::Error(NoBlackKing)					"Sin rey negro."
::setup::board::mc::Error(DoubleCheck)					"Ambos reyes en jaque."
::setup::board::mc::Error(OppositeCheck)				"El lado que no mueve est� en jaque."
::setup::board::mc::Error(TooManyWhitePawns)			"Demasiados peones blancos."
::setup::board::mc::Error(TooManyBlackPawns)			"Demasiados peones negros."
::setup::board::mc::Error(TooManyWhitePieces)		"Demasiadas piezas blancas."
::setup::board::mc::Error(TooManyBlackPieces)		"Demasiadas piezas negras."
::setup::board::mc::Error(PawnsOn18)					"Pe�n en la 1ra o en la 8va fila."
::setup::board::mc::Error(TooManyKings)				"M�s de dos reyes."
::setup::board::mc::Error(TooManyWhite)				"Demasiadas piezas blancas."
::setup::board::mc::Error(TooManyBlack)				"Demasiadas piezas negras."
::setup::board::mc::Error(BadCastlingRights)			"Derechos de enroque equivocados."
::setup::board::mc::Error(InvalidCastlingRights)	"Fila(s) de la torre irrazonable para el enroque."
::setup::board::mc::Error(InvalidCastlingFile)		"Fila no v�lida para el enroque."
::setup::board::mc::Error(AmbiguousCastlingFyles)	"El enroque requiere filas con torre para no ser ambiguo (posiblemente est�n mal configuradas)."
::setup::board::mc::Error(InvalidEnPassant)			"Fila al paso no razonable."
::setup::board::mc::Error(MultiPawnCheck)				"Dos o m�s peones dando jaque."
::setup::board::mc::Error(TripleCheck)					"Tres o m�s piezas dando jaque."

### import #############################################################
::import::mc::ImportingPgnFile					"Importar archivo PGN '%s'"
::import::mc::Line									"L�nea"
::import::mc::Column									"Columna"
::import::mc::GameNumber							"Partida"
::import::mc::ImportedGames						"%s partida(s) importada(s)"
::import::mc::NoGamesImported						"Ninguna partida importada"
::import::mc::FileIsEmpty							"el archivo probablemente est� vac�o"
::import::mc::PgnImport								"Importar PGN"
::import::mc::ImportPgnGame						"Importar partida PGN"
::import::mc::ImportPgnVariation					"Importar variante PGN"
::import::mc::ImportOK								"Texto PGN importado sin errores o advertencias."
::import::mc::ImportAborted						"Importaci�n abortada."
::import::mc::TextIsEmpty							"El texto PGN est� vac�o."
::import::mc::AbortImport							"�Abortar importaci�n de PGN?"

::import::mc::DifferentEncoding					"Selected encoding %src does not match file encoding %dst." ;# NEW
::import::mc::DifferentEncodingDetails			"Recoding of the database will not be successful anymore after this action." ;# NEW
::import::mc::CannotDetectFigurineSet			"Cannot auto-detect a suitable figurine set." ;# NEW
::import::mc::CheckImportResult					"Please check whether the right figurine set is detected." ;# NEW
::import::mc::CheckImportResultDetail			"In seldom cases the auto-detection fails due to ambiguities." ;# NEW

::import::mc::EnterOrPaste							"Ingrese o pegue en formato PGN %s en el cuadro de arriba.\nCualquier falla al importar el %s se mostrar� aqu�."
::import::mc::EnterOrPaste-Game					"partida"
::import::mc::EnterOrPaste-Variation			"variante"

::import::mc::MissingWhitePlayerTag				"Jugador de las Blancas desconocido"
::import::mc::MissingBlackPlayerTag				"Jugador de las Negras desconocido"
::import::mc::MissingPlayerTags					"Jugadores desconocidos"
::import::mc::MissingResult						"Resultado desconocido (al final de la secci�n de jugadas)"
::import::mc::MissingResultTag					"Resultado desconocido (en la secci�n encabezado)"
::import::mc::InvalidRoundTag						"Ronda no v�lida en el encabezado"
::import::mc::InvalidResultTag					"Resultado no v�lido en el encabezado"
::import::mc::InvalidDateTag						"Fecha no v�lida en el encabezado"
::import::mc::InvalidEventDateTag				"Fecha del evento no v�lida en el encabezado"
::import::mc::InvalidTimeModeTag					"Par�metros de tiempo no v�lidos en el encabezado"
::import::mc::InvalidEcoTag						"ECO no v�lido en el encabezado"
::import::mc::InvalidTagName						"Nombre no v�lido en el encabezado (ignorado)"
::import::mc::InvalidCountryCode					"C�digo de pa�s no v�lido"
::import::mc::InvalidRating						"N�mero de rating no v�lido"
::import::mc::InvalidNag							"NAG no v�lido"
::import::mc::BraceSeenOutsideComment			"\"\}\" fuera de un comentario en la partida (se ignorar�n)" 
::import::mc::MissingFen							"FEN desconocido (el encabezado de variante se ignorar�)"
::import::mc::UnknownEventType					"Tipo de evento desconocido"
::import::mc::UnknownTitle							"T�tulo desconocido (ignored)"
::import::mc::UnknownPlayerType					"Unknown player type (ignored)"
::import::mc::UnknownSex							"Unknown sex (ignorado)"
::import::mc::UnknownTermination					"Motivo de la terminaci�n desconocido"
::import::mc::UnknownMode							"Modo desconocido"
::import::mc::RatingTooHigh						"N�mero de valuaci�n demasiado alto (ignorado)"
::import::mc::TooManyNags							"Demasiados NAG's (se ignorar� el �ltimo)"
::import::mc::IllegalCastling						"Enroque ilegal"
::import::mc::IllegalMove							"Jugada ilegal"
::import::mc::UnsupportedVariant					"Variante de ajedrez no soportada"
::import::mc::DecodingFailed						"Decoding of this game was not possible" ;# NEW
::import::mc::ResultDidNotMatchHeaderResult	"El resultado no es igual al resultado del encabezado"
::import::mc::ValueTooLong							"El encabezado es demasiado largo y se cortar� a los 255 caracteres"
::import::mc::MaximalErrorCountExceeded		"M�ximo de errores excedido; no se informar�n m�s errores (del tipo de error previo)"
::import::mc::MaximalWarningCountExceeded		"M�ximo de advertencias excedido; no se informar�n m�s advertencias (del tipo previo de advertencias)"
::import::mc::InvalidToken							"S�mbolo no v�lido"
::import::mc::InvalidMove							"Jugada no v�lida"
::import::mc::UnexpectedSymbol					"S�mbolo inesperado"
::import::mc::UnexpectedEndOfInput				"Inesperado final de la entrada de datos"
::import::mc::UnexpectedResultToken				"S�mbolo de resultado inesperado"
::import::mc::UnexpectedTag						"Etiqueta inesperada dentro de la partida"
::import::mc::UnexpectedEndOfGame				"Final de la partida inesperado (resultado desconocido)"
::import::mc::TagNameExpected						"Error de sintaxis: se esperaba un nombre en el encabezado"
::import::mc::TagValueExpected					"Error de sintaxis: Se esperaba un valor en el encabezado"
::import::mc::InvalidFen							"FEN no v�lido"
::import::mc::UnterminatedString					"Flujo no determinado"
::import::mc::UnterminatedVariation				"Variante no determinada"
::import::mc::TooManyGames							"Demasiadas partidas en la base (abortado)"
::import::mc::GameTooLong							"Partida demasiado larga (omitida)"
::import::mc::FileSizeExceeded					"Se excede el tama�o m�ximo de archivo (2GB) (abortado)"
::import::mc::TooManyPlayerNames					"Demasiados nombres de jugadores en la base (abortado)"
::import::mc::TooManyEventNames					"Demasiados nombres de evento en la base (abortado)"
::import::mc::TooManySiteNames					"Demasiados nombres de lugares en la base (abortado)"
::import::mc::TooManyRoundNames					"Demasiados nombres de ronda en la base (abortado)"
::import::mc::TooManyAnnotatorNames				"Demasiados nombres de comentaristas en la base (abortado)"
::import::mc::TooManySourceNames					"Demasiados nombres de or�genes en la base (abortado)"
::import::mc::SeemsNotToBePgnText				"No parece ser un texto PGN"

### export #############################################################
::export::mc::FileSelection				"&Selecci�n de archivo"
::export::mc::OptionsSetup					"&Opciones"
::export::mc::PageSetup						"&Configuraci�n de p�gina"
::export::mc::DiagramSetup					"&Diagram Setup" ;# NEW
::export::mc::StyleSetup					"&Estilo"
::export::mc::EncodingSetup				"Cod&ificaci�n"
::export::mc::TagsSetup						"&Tags"
::export::mc::NotationSetup				"&Notaci�n"
::export::mc::AnnotationSetup				"Not&as"
::export::mc::CommentsSetup				"Co&mentarios"

::export::mc::Visibility					"Visibility" ;# NEW
::export::mc::HideDiagrams					"Hide Diagrams" ;# NEW
::export::mc::AllFromWhitePersp			"All From White's Perspective" ;# NEW
::export::mc::AllFromBlackPersp			"All From Black's Perspective" ;# NEW
::export::mc::ShowCoordinates				"Mostrar coordenadas"
::export::mc::ShowSideToMove				"Mostrar el lado que mueve"
::export::mc::ShowArrows					"Show Arrows" ;# NEW
::export::mc::ShowMarkers					"Show Markers" ;# NEW
::export::mc::Layout							"Disposici�n"
::export::mc::PostscriptSpecials			"Postscript Specialities" ;# NEW
::export::mc::BoardSize						"Board Size" ;# NEW

::export::mc::Notation						"Notaci�n"
::export::mc::Graphic						"Gr�ficos"
::export::mc::Short							"Corto"
::export::mc::Long							"Largo"
::export::mc::Algebraic						"Algebraico"
::export::mc::Correspondence				"Correspondencia"
::export::mc::Telegraphic					"Telegr�fico"
::export::mc::FontHandling					"Manejo de fuentes"
::export::mc::DiagramStyle					"Diagram Style" ;# NEW
::export::mc::UseImagesForDiagram		"Use images for diagram generation" ;# NEW
::export::mc::EmebedTruetypeFonts		"Empotrar fuentes TrueType"
::export::mc::UseBuiltinFonts				"Usar fuentes incluidas"
::export::mc::SelectExportedTags			"Selection of exported tags" ;# NEW
::export::mc::ExcludeAllTags				"Exclude all tags" ;# NEW
::export::mc::IncludeAllTags				"Include all tags" ;# NEW
::export::mc::ExtraTags						"All other extra tags" ;# NEW
::export::mc::NoComments					"No comments" ;# NEW
::export::mc::AllLanguages					"All languages" ;# NEW
::export::mc::Significant					"Significant" ;# NEW
::export::mc::LanguageSelection			"Language selection" ;# NEW
::export::mc::MapTo							"Map to" ;# NEW
::export::mc::MapNagsToComment			"Map annotations to comments" ;# NEW
::export::mc::UnusualAnnotation			"Unusual annotations" ;# NEW
::export::mc::AllAnnotation				"All annotations" ;# NEW
::export::mc::UseColumnStyle				"Use column style" ;# NEW
::export::mc::MainlineStyle				"Main Line Style" ;# NEW
::export::mc::HideVariations				"Hide variations" ;# NEW

::export::mc::PdfFiles						"Archivos PDF"
::export::mc::HtmlFiles						"Archivos HTML"
::export::mc::TeXFiles						"Archivos LaTeX"

::export::mc::ExportDatabase				"Exportar base"
::export::mc::ExportDatabaseTitle		"Exportar base '%s'"
::export::mc::ExportingDatabase			"Exportando '%s' al archivo '%s'"
::export::mc::Export							"Exportar"
::export::mc::ExportedGames				"%s partida(s) exportada(s)"
::export::mc::NoGamesForExport			"No hay partidas para exportar."
::export::mc::ResetDefaults				"Volver a los par�metros predeterminados"
::export::mc::UnsupportedEncoding		"No use la codificaci�n %s para documentos PDF. Debe elegir una codificaci�n alternativa."
::export::mc::DatabaseIsOpen				"La base '%s' est� abierta. Debe cerrarla primero."

::export::mc::BasicStyle					"Estilo b�sico"
::export::mc::GameInfo						"Informaci�n de la partida"
::export::mc::GameText						"Texto de la partida"
::export::mc::Moves							"Jugadas"
::export::mc::MainLine						"L�nea principal"
::export::mc::Variation						"Variante"
::export::mc::Subvariation					"Subvariante"
::export::mc::Figurines						"Figurines"
::export::mc::Hyphenation					"Hyphenation" ;# NEW
::export::mc::None							"(ninguno)"
::export::mc::Symbols						"Simbolos"
::export::mc::Comments						"Comentarios"
::export::mc::Result							"Resultado"
::export::mc::Diagram						"Diagrama"
::export::mc::ColumnStyle					"Column Style" ;# NEW

::export::mc::Paper							"Papel"
::export::mc::Orientation					"Orientaci�n"
::export::mc::Margin							"M�rgenes"
::export::mc::Format							"Formato"
::export::mc::Size							"Tama�o"
::export::mc::Custom							"Habitual"
::export::mc::Potrait						"Retrato"
::export::mc::Landscape						"Apaisado"
::export::mc::Top								"Superior"
::export::mc::Bottom							"Inferior"
::export::mc::Left							"Izquierda"
::export::mc::Right							"Derecha"
::export::mc::Justification				"Justificado"
::export::mc::Even							"Ajustado"
::export::mc::Columns						"Columnas"
::export::mc::One								"Una"
::export::mc::Two								"Dos"

::export::mc::DocumentStyle				"Document Style" ;# NEW
::export::mc::Article						"Article" ;# NEW
::export::mc::Report							"Report" ;# NEW
::export::mc::Book							"Book" ;# NEW

::export::mc::FormatName(scidb)			"Scidb"
::export::mc::FormatName(scid)			"Scid"
::export::mc::FormatName(pgn)				"PGN"
::export::mc::FormatName(pdf)				"PDF"
::export::mc::FormatName(html)			"HTML"
::export::mc::FormatName(tex)				"LaTeX"
::export::mc::FormatName(ps)				"Postscript"

::export::mc::Option(pgn,include_varations)						"Exportar variantes"
::export::mc::Option(pgn,include_comments)						"Exportar comentarios"
::export::mc::Option(pgn,include_moveinfo)						"Export move information (as comments)" ;# NEW
::export::mc::Option(pgn,include_marks)							"Exportar marcadores (como comentarios)"
::export::mc::Option(pgn,use_scidb_import_format)				"Usar el formato de importaci�n de Scidb"
::export::mc::Option(pgn,use_chessbase_format)					"Usar formato ChessBase"
::export::mc::Option(pgn,include_ply_count_tag)					"Escribir la etiqueta 'PlyCount'"
::export::mc::Option(pgn,include_termination_tag)				"Escribir etiqueta 'Termination'"
::export::mc::Option(pgn,include_mode_tag)						"Escribir etiqueta 'Mode'"
::export::mc::Option(pgn,include_opening_tag)					"Escribir etiquetas 'Opening', 'Variation', 'Subvariation'"
::export::mc::Option(pgn,include_setup_tag)						"Escribir etiqueta 'Setup' (si es necesario)"
::export::mc::Option(pgn,include_variant_tag)					"Escribir etiqueta 'Variant' (si es necesario)"
::export::mc::Option(pgn,include_position_tag)					"Escribir etiqueta 'Position' (si es necesario)"
::export::mc::Option(pgn,include_time_mode_tag)					"Escribir etiqueta 'TimeMode' (si es necesario)"
::export::mc::Option(pgn,exclude_extra_tags)						"Excluir etiquetas superfluas"
::export::mc::Option(pgn,indent_variations)						"Sangrar variantes"
::export::mc::Option(pgn,indent_comments)							"Sangrar comentarios"
::export::mc::Option(pgn,column_style)								"Estilo columna (una jugada por l�nea)"
::export::mc::Option(pgn,symbolic_annotation_style)			"Estilo simb�lico de comentarios (!, !?)"
::export::mc::Option(pgn,extended_symbolic_style)				"Estilo simb�lico extendido de comentarios (+=, +/-)"
::export::mc::Option(pgn,convert_null_moves)						"Convertir las jugadas nulas en comentarios"
::export::mc::Option(pgn,space_after_move_number)				"Agregar espacio tras los n�meros de jugada"
::export::mc::Option(pgn,shredder_fen)								"Escribir Shredder-FEN (X-FEN es lo predeterminado)"
::export::mc::Option(pgn,convert_lost_result_to_comment)		"Escribir comentario para el resultado '0-0'"
::export::mc::Option(pgn,append_mode_to_event_type)			"Agregar modo tras el tipo de evento"
::export::mc::Option(pgn,comment_to_html)							"Escribir comentario en estilo HTML"
::export::mc::Option(pgn,exclude_games_with_illegal_moves)	"Excluir partidas con jugadas ilegales"

### save/replace #######################################################
::dialog::save::mc::SaveGame						"Guardar partida"
::dialog::save::mc::ReplaceGame					"Reemplazar partida"
::dialog::save::mc::EditCharacteristics		"Editar caracter�sticas"
	
::dialog::save::mc::GameData						"Datos de la partida"
::dialog::save::mc::Event							"Evento"

::dialog::save::mc::MatchesExtraTags			"Etiquetas iguales / Extra Tags" ;# NEW (changed)
::dialog::save::mc::PressToSelect				"Presione Ctrl+0 a Ctrl+9 (o el bot�n izquierdo del rat�n) para seleccionar"
::dialog::save::mc::PressForWhole				"Presione Alt-0 a Alt-9 (o el bot�n medio del rat�n) para grupo de datos completo"
::dialog::save::mc::EditTags						"Editar etiquetas"
::dialog::save::mc::DeleteThisTag				"Eliminar etiqueta '%s'?"
::dialog::save::mc::TagAlreadyExists			"El nombre de etiqueta '%s' ya existe."
::dialog::save::mc::TagDeleted					"La etiqueta sobrante '%s' (valor actual: '%s') ser� eliminada."
::dialog::save::mc::TagNameIsReserved			"El nombre de etiqueta '%s' est� reservado."
::dialog::save::mc::Locked							"Bloqueado"
::dialog::save::mc::OtherTag						"Otra etiqueta"
::dialog::save::mc::NewTag							"Nueva etiqueta"
::dialog::save::mc::DeleteTag						"Etiqueta eliminada"
::dialog::save::mc::SetToGameDate				"Establecer fecha de la partida"
::dialog::save::mc::SaveGameFailed				"Guardado de la partida fallido."
::dialog::save::mc::SaveGameFailedDetail		"Ver bit�cora para los detalles."
::dialog::save::mc::SavingGameLogInfo			"Guardar partida (%white - %black, %event) en base '%base'"
::dialog::save::mc::CurrentBaseIsReadonly		"La base actual '%s' es de s�lo lectura."
::dialog::save::mc::CurrentGameHasTrialMode	"Current game is in trial mode and cannot be saved." ;# NEW

::dialog::save::mc::LocalName						"&Nombre local"
::dialog::save::mc::EnglishName					"Nombre I&ngl�s"
::dialog::save::mc::ShowRatingType				"Mostrar &rating"
::dialog::save::mc::EcoCode						"C�digo &ECO"
::dialog::save::mc::Matches						"&Matches"
::dialog::save::mc::Tags							"E&tiquetas"

::dialog::save::mc::Name							"Nombre"
::dialog::save::mc::NameFideID					"Nombre/Fide-ID"
::dialog::save::mc::Value							"Valor"
::dialog::save::mc::Title							"T�tulo"
::dialog::save::mc::Rating							"Rating"
::dialog::save::mc::Federation					"Federaci�n"
::dialog::save::mc::Country						"Pa�s"
::dialog::save::mc::Type							"Tipo"
::dialog::save::mc::Sex								"Sexo/Tipo"
::dialog::save::mc::Date							"Fecha"
::dialog::save::mc::EventDate						"Fecha del evento"
::dialog::save::mc::Round							"Ronda"
::dialog::save::mc::Result							"Resultado"
::dialog::save::mc::Termination					"Terminaci�n"
::dialog::save::mc::Annotator						"Comentarista"
::dialog::save::mc::Site							"Lugar"
::dialog::save::mc::Mode							"Modo"
::dialog::save::mc::TimeMode						"Tiempo"
::dialog::save::mc::Frequency						"Frecuencia"

::dialog::save::mc::GameBase						"Base de partidas"
::dialog::save::mc::PlayerBase					"Base de jugadores"
::dialog::save::mc::EventBase						"Base de eventos"
::dialog::save::mc::SiteBase						"Base de lugares"
::dialog::save::mc::AnnotatorBase				"Base de comentaristas"
::dialog::save::mc::History						"Historial"

::dialog::save::mc::InvalidEntry					"'%s' no es una entrada v�lida."
::dialog::save::mc::InvalidRoundEntry			"'%s' no es una entrada de ronda v�lida."
::dialog::save::mc::InvalidRoundEntryDetail	"Entradas de ronda v�lidas son '4' � '6.1'. No se permiten ceros."
::dialog::save::mc::RoundIsTooHigh				"La ronda deber�a ser menor a 256."
::dialog::save::mc::SubroundIsTooHigh			"La sub-ronda deber�a ser menor a 256."
::dialog::save::mc::ImplausibleDate				"La fecha de la partida '%s' es anterior a la fecha del evento '%s'."
::dialog::save::mc::InvalidTagName				"Nombre de etiqueta no v�lido '%s' (error de sintaxis)."
::dialog::save::mc::Field							"Campo '%s': "
::dialog::save::mc::ExtraTag						"Etiqueta extra '%s': "
::dialog::save::mc::InvalidNetworkAddress		"Direcci�n de red '%s' no v�lida."
::dialog::save::mc::InvalidCountryCode			"C�digo de pa�s '%s' no v�lido."
::dialog::save::mc::InvalidEventRounds			"N�mero de rondas del evento '%s' no v�lido (se espera un n�mero entero positivo)."
::dialog::save::mc::InvalidPlyCount				"Recuento de jugadas '%s' no v�lido (se espera un n�mero entero positivo)."
::dialog::save::mc::IncorrectPlyCount			"Recuento de jugadas '%s' incorrecto (el recuento de jugadas real es %s)."
::dialog::save::mc::InvalidTimeControl			"Entrada del campo de control de tiempo en '%s' no v�lida."
::dialog::save::mc::InvalidDate					"Fecha '%s' no v�lida."
::dialog::save::mc::InvalidYear					"A�o '%s' no v�lido."
::dialog::save::mc::InvalidMonth					"Mes '%s' no v�lido."
::dialog::save::mc::InvalidDay					"D�a '%s' no v�lido."
::dialog::save::mc::MissingYear					"Se desconoce el a�o."
::dialog::save::mc::MissingMonth					"Se desconoce el mes."
::dialog::save::mc::StringTooLong				"Etiqueta %tag%: la cadena '%value%' es demasiado larga y se cortar� a '%trunc%'."
::dialog::save::mc::InvalidEventDate			"No se puede aceptar la fecha de evento suministrada: La diferencia entre el a�o de la partida y el a�o del evento deber�a ser menor a 4 (restricci�n del formato de base de datos de Scid)."
::dialog::save::mc::TagIsEmpty					"La etiqueta '%s' est� vac�a (se descartar�)."

### gamehistory ########################################################
::game::history::mc::GameHistory	"Game History" ;# NEW

### game ###############################################################
::game::mc::CloseDatabase				"Cerrar Base"
::game::mc::CloseAllGames				"�Cerrar todas las partidas abiertas de la base '%s'?"
::game::mc::SomeGamesAreModified		"Se modificaron algunas partidas de la base '%s'. �Cerrar de todos modos?" 
::game::mc::AllSlotsOccupied			"Todos los espacios para partidas est�n ocupados."
::game::mc::ReleaseOneGame				"Por favor,  saque una de las partidas antes de cargar otra."
::game::mc::GameAlreadyOpen			"La partida ya est� abierta pero fue modificada. �Descartar la versi�n modificada de esta partida?"
::game::mc::GameAlreadyOpenDetail	"'%s' abrir� una nueva partida."
::game::mc::GameHasChanged				"La partida %s se ha modificado."
::game::mc::GameHasChangedDetail		"Probably this is not the expected game due to database changes."
::game::mc::CorruptedHeader			"Encabezado corrupto en el archivo de recuperaci�n '%s'."
::game::mc::RenamedFile					"Renombrar este archivo como '%s.bak'."
::game::mc::CannotOpen					"No se puede abrir el archivo de recuperaci�n '%s'."
::game::mc::GameRestored				"Una partida restablecida de la �ltima sesi�n."
::game::mc::GamesRestored				"%s partidas restablecidas de la �ltima sesi�n."
::game::mc::OldGameRestored			"Una partida restablecida."
::game::mc::OldGamesRestored			"%s partidas restablecidas."
::game::mc::ErrorInRecoveryFile		"Error en el archivo de recuperaci�n '%s'"
::game::mc::Recovery						"Recuperaci�n"
::game::mc::UnsavedGames				"Usted tiene cambios en la partida que no se han guardado."
::game::mc::DiscardChanges				"'%s' descartar� todos los cambios."
::game::mc::ShouldRestoreGame			"�Deber�a restablecerse esta partida en la pr�xima sesi�n?"
::game::mc::ShouldRestoreGames		"�Deber�an restablecerse estas partidas en la sesi�n siguiente?"
::game::mc::NewGame						"Nueva partida"
::game::mc::NewGames						"Nuevas partidas"
::game::mc::Created						"creado"
::game::mc::ClearHistory				"Clear History" ;# NEW
::game::mc::RemoveSelectedGame		"Remove selected game from history" ;# NEW
::game::mc::GameDataCorrupted			"Los datos de la partida son err�neos."
::game::mc::GameDecodingFailed		"Decoding of this game was not possible." ;# NEW

### languagebox ########################################################
::languagebox::mc::AllLanguages	"Todos los idiomas"
::languagebox::mc::None				"None" ;# NEW

### datebox ############################################################
::widget::datebox::mc::Today		"Hoy"
::widget::datebox::mc::Calendar	"Calendario..."
::widget::datebox::mc::Year		"A�o"
::widget::datebox::mc::Month		"Mes"
::widget::datebox::mc::Day			"D�a"

### genderbox ##########################################################
::genderbox::mc::Gender(m) "Masculino"
::genderbox::mc::Gender(f) "Femenino"
::genderbox::mc::Gender(c) "Computadora"

### terminationbox #####################################################
::terminationbox::mc::Normal				"Normal"
::terminationbox::mc::Unplayed			"Unplayed" ;# NEW
::terminationbox::mc::Abandoned			"Abandona"
::terminationbox::mc::Adjudication		"Adjudicaci�n"
::terminationbox::mc::Death				"Muerte"
::terminationbox::mc::Emergency			"Emergencia"
::terminationbox::mc::RulesInfraction	"Infracci�n a las reglas"
::terminationbox::mc::TimeForfeit		"Pierde por tiempo"
::terminationbox::mc::Unterminated		"No terminada"

### eventmodebox #######################################################
::eventmodebox::mc::OTB				"Sobre el tablero"
::eventmodebox::mc::PM				"Correspondencia"
::eventmodebox::mc::EM				"E-mail"
::eventmodebox::mc::ICS				"Internet Chess Server"
::eventmodebox::mc::TC				"Telecomunicaci�n"
::eventmodebox::mc::Analysis		"An�lisis"
::eventmodebox::mc::Composition	"Composici�n"

### eventtypebox #######################################################
::eventtypebox::mc::Type(game)	"Partida individual"
::eventtypebox::mc::Type(match)	"Match"
::eventtypebox::mc::Type(tourn)	"Round Robin"
::eventtypebox::mc::Type(swiss)	"Torneo por sistema suizo"
::eventtypebox::mc::Type(team)	"Torneo por equipos"
::eventtypebox::mc::Type(k.o.)	"Torneo por Knockout"
::eventtypebox::mc::Type(simul)	"Torneo de simult�neas"
::eventtypebox::mc::Type(schev)	"Torneo por sistema Scheveningen"  

### timemodebox ########################################################
::timemodebox::mc::Mode(normal)	"Normal"
::timemodebox::mc::Mode(rapid)	"R�pidas"
::timemodebox::mc::Mode(blitz)	"Blitz"
::timemodebox::mc::Mode(bullet)	"Bullet"
::timemodebox::mc::Mode(corr)		"Correspondencia"

### crosstable #########################################################
::crosstable::mc::TournamentTable		"Grilla de torneo"
::crosstable::mc::AverageRating			"Rating promedio"
::crosstable::mc::Category					"Categor�a"
::crosstable::mc::Games						"partidas"
::crosstable::mc::Game						"partida"

::crosstable::mc::ScoringSystem			"Scoring System" ;# NEW
::crosstable::mc::Tiebreak					"Desempate"
::crosstable::mc::Settings					"Configuraciones"
::crosstable::mc::RevertToStart			"Volver a los valores iniciales"
::crosstable::mc::UpdateDisplay			"Actualizar el visor"

::crosstable::mc::Traditional				"Traditional" ;# NEW
::crosstable::mc::Bilbao					"Bilbao" ;# NEW

::crosstable::mc::None						"Ninguno"
::crosstable::mc::Buchholz					"Buchholz"
::crosstable::mc::MedianBuchholz			"Buchholz-Mediano"
::crosstable::mc::ModifiedMedianBuchholz "Buchholz-Mediano Mod."
::crosstable::mc::RefinedBuchholz		"Buchholz perfeccionado"
::crosstable::mc::SonnebornBerger		"Sonneborn-Berger"
::crosstable::mc::Progressive				"Puntajes progresivos"
::crosstable::mc::KoyaSystem				"Sistema Koya"
::crosstable::mc::GamesWon					"N�mero de partidas ganadas"
::crosstable::mc::GamesWonWithBlack		"Games Won with Black" ; # NEW
::crosstable::mc::ParticularResult		"Particular Result" ;# NEW
::crosstable::mc::TraditionalScoring	"Traditional Scoring" ;# NEW

::crosstable::mc::Crosstable				"Cuadro cruzado"
::crosstable::mc::Scheveningen			"Scheveningen"
::crosstable::mc::Swiss						"Sistema suizo"
::crosstable::mc::Match						"Match"
::crosstable::mc::Knockout					"Knockout"
::crosstable::mc::RankingList				"Lista de Ranking"

::crosstable::mc::Order						"Orden"
::crosstable::mc::Type						"Tipo tabla"
::crosstable::mc::Score						"Puntuaci�n"
::crosstable::mc::Alphabetical			"Alfab�tico"
::crosstable::mc::Rating					"Rating"
::crosstable::mc::Federation				"Federaci�n"

::crosstable::mc::Debugging				"Depuraci�n"
::crosstable::mc::Display					"Visor"
::crosstable::mc::Style						"Estilo"
::crosstable::mc::Spacing					"Espaciado"
::crosstable::mc::Padding					"Relleno"
::crosstable::mc::ShowLog					"Mostrar bit�cora"
::crosstable::mc::ShowHtml					"Mostrar HTML"
::crosstable::mc::ShowRating				"Rating"
::crosstable::mc::ShowPerformance		"Desempe�o"
::crosstable::mc::ShowTiebreak			"Desempate"
::crosstable::mc::ShowOpponent			"Oponente (como Tooltip)"
::crosstable::mc::KnockoutStyle			"Estilo tabla de Knockout"
::crosstable::mc::Pyramid					"Pir�mide"
::crosstable::mc::Triangle					"Tri�ngulo"

::crosstable::mc::CrosstableLimit		"Se exceder� el l�mite del cuadro cruzado de %d jugadores."
::crosstable::mc::CrosstableLimitDetail "'%s' est� seleccionando otro modo de tabla."

### info ###############################################################
::info::mc::InfoTitle			"Acerca de %s"
::info::mc::Info					"Informaci�n"
::info::mc::About					"Acerca de"
::info::mc::Contributions		"Contribuciones"
::info::mc::License				"Licencia"
::info::mc::Localization		"Localizaci�n"
::info::mc::Testing				"Pruebas"
::info::mc::References			"Referencias"
::info::mc::System				"Sistema"
::info::mc::FontDesign			"Dise�o de fuentes"
::info::mc::ChessPieceDesign	"Dise�o de las piezas"
::info::mc::BoardThemeDesign	"Dise�o de tema de tablero"
::info::mc::FlagsDesign			"Dise�o de las banderas en miniatura"
::info::mc::IconDesign			"Dise�o de iconos"

::info::mc::Version				"Versi�n"
::info::mc::Distributed			"Este programa se distribuye bajo los t�rminos de la Licencia P�blica General GNU."
::info::mc::Inspired				"Scidb est� inspirado en Scid 3.6.1, registrado en \u00A9 1999-2003 por Shane Hudson."
::info::mc::SpecialThanks		"Un especial agradecimiento a Shane Hudson por su estupendo trabajo. Su empe�o constituye la base de esta aplicaci�n."

::info::mc::Reference(PGN)			"es el est�ndar aceptado para la representaci�n textual de partidas de ajedrez y su transferencia entre bases de datos ajedrec�sticas. Este est�ndar fue creado por Steven J. Edwards y el documento que lo explica est� disponible en muchos sitios web de ajedrez; �sta es una de las ubicaciones: %url%."
::info::mc::Reference(Crafty)		"es uno de los programas de ajedrez gratuitos m�s fuertes. Su autor es Bob Hyatt. El sitio ftp de Crafty es: %url%. El subdirectorio \"TB\" de este sitio contiene muchos archivos de tablebase, que tambi�n pueden usarse en Scidb."
::info::mc::Reference(Stockfish)	"es un motor de ajedrez de c�digo abierto basado en Glaurung. Probablemente sea el motor de ajedrez gratuito disponible m�s fuerte. Stockfish puede bajarse desde %url%"
::info::mc::Reference(Toga)		"probablemente sea el motor de ajedrez gratuito m�s fuerte disponible. Los autores son Thomas Gaksch y Fabien Letouzey. El sitio de Toga II es %url%."
::info::mc::Reference(Fruit)		"Fruit es un motor de ajedrez desarrollado por Fabien Letouzey y Joachim Rang, y es el sub campe�n mundial de ajedrez por computadoras del 2005. Este motor soporta el Chess960 y gan� dos veces la Liga de Motores de Chess960. El sitio de Fruit es %url%."
::info::mc::Reference(Phalanx)	"El estilo de juego de Phalanx es muy parecido al humano; cuando juega en toda su potencia puede compararse a un jugador de club intermedio a fuerte; los principiantes tambi�n se sentir�n en casa con �l. El autor de Phalanx es Dusan Dobes. Puede encontrar el motor de ajedrez en %url%."
::info::mc::Reference(Gully)		"El programa de ajedrez Gullydeckel le permite jugar una partida de ajedrez contra un oponente no tan poderoso. Ha sido escrito por Martin Borriss. El sitio de Gullydeckel es %url%."
::info::mc::Reference(MicroMax)	"es posiblemente el programa de ajedrez escrito en C m�s peque�o que existe. El sitio de Micro-Max es %url%. Micro-Max fue escrito por H.G. Muller."

### comment ############################################################
::comment::mc::CommentBeforeMove		"Comentario antes de la jugada"
::comment::mc::CommentAfterMove		"Comentario tras la jugada"
::comment::mc::PrecedingComment		"Comentario precedente"
::comment::mc::TrailingComment		"�ltimo comentario"
::comment::mc::Language					"Idioma"
::comment::mc::AddLanguage				"Agregar idioma..."
::comment::mc::SwitchLanguage			"Cambiar idioma"
::comment::mc::FormatText				"Dar formato al texto"
::comment::mc::CopyText					"Copy text to" ;# NEW
::comment::mc::OverwriteContent		"Overwrite existing content?" ;# NEW
::comment::mc::AppendContent			"If \"no\" the text will be appended." ;# NEW

::comment::mc::Bold						"Negrita"
::comment::mc::Italic					"It�lica"
::comment::mc::Underline				"Subrayado"

::comment::mc::InsertSymbol			"&Insertar s�mbolo..."
::comment::mc::MiscellaneousSymbols	"Simbolos miscel�neos"
::comment::mc::Figurine					"Figurines"

### annotation #########################################################
::annotation::mc::AnnotationEditor					"Notas"
::annotation::mc::TooManyNags							"Demasiados comentarios (el �ltimo ser� ignorado)."
::annotation::mc::TooManyNagsDetail					"Se permite un m�ximo de %d comentarios por jugada individual."

::annotation::mc::PrefixedCommentaries				"Comentarios prefijados"
::annotation::mc::MoveAssesments						"Evaluaciones de jugadas"
::annotation::mc::PositionalAssessments			"Evaluaciones posicionales"
::annotation::mc::TimePressureCommentaries		"Comentarios sobre apuros de tiempo"
::annotation::mc::AdditionalCommentaries			"Comentarios adicionales"
::annotation::mc::ChessBaseCommentaries			"Comentarios de ChessBase"

### marks ##############################################################
::marks::mc::MarksPalette			"Paleta de marcadores"

### move ###############################################################
::move::mc::ReplaceMove				"Reemplazar jugada"
::move::mc::AddNewVariation		"Agregar nueva variante"
::move::mc::NewMainLine				"Nueva l�nea principal"
::move::mc::TryVariation			"Probar variante"
::move::mc::ExchangeMove			"Cambiar jugada"

::move::mc::GameWillBeTruncated	"Se truncar� la partida. �Continuar con '%s'?"

### log ################################################################
::log::mc::LogTitle		"Bit�cora"
::log::mc::Warning		"Advertencia"
::log::mc::Error			"Error"
::log::mc::Information	"Informaci�n"

### titlebox ############################################################
::titlebox::mc::Title(GM)	"Gran Maestro (FIDE)"
::titlebox::mc::Title(IM)	"Maestro Internacional (FIDE)"
::titlebox::mc::Title(FM)	"Maestro Fide (FIDE)"
::titlebox::mc::Title(CM)	"Candidato a Maestro (FIDE)"
::titlebox::mc::Title(WGM)	"Gran Maestro Femenino (FIDE)"
::titlebox::mc::Title(WIM)	"Maestro Internacional Femenino (FIDE)"
::titlebox::mc::Title(WFM)	"Maestro Fide Femenino (FIDE)"
::titlebox::mc::Title(WCM)	"Candidato a Maestro Femenino (FIDE)"
::titlebox::mc::Title(HGM)	"Gran Maestro Honorario (FIDE)"
::titlebox::mc::Title(NM)	"Maestro Nacional (USCF)"
::titlebox::mc::Title(SM)	"Maestro Senior (USCF)"
::titlebox::mc::Title(LM)	"Life Master (USCF)"
::titlebox::mc::Title(CGM)	"Gran Maestro por Correspondencia (ICCF)"
::titlebox::mc::Title(CSM)	"Maestro Internacional Senior por Correspondencia (ICCF)"
::titlebox::mc::Title(CIM)	"Maestro Internacional por Correspondencia (ICCF)"

### messagebox #########################################################
::dialog::mc::Ok				"&Aceptar"
::dialog::mc::Cancel			"&Cancelar"
::dialog::mc::Yes				"&S�"
::dialog::mc::No				"&No"
::dialog::mc::Retry			"&Reintentar"
::dialog::mc::Abort			"A&bortar"
::dialog::mc::Ignore			"&Ignorar"
::dialog::mc::Continue		"Con&tinue" ;# NEW

::dialog::mc::Error			"Error"
::dialog::mc::Warning		"Advertencia"
::dialog::mc::Information	"Informaci�n"
::dialog::mc::Question		"Pregunta"

::dialog::mc::DontAskAgain	"No pregunte nuevamente"

### web ################################################################
::web::mc::CannotFindBrowser			"Couldn't find a suitable web browser." ;# NEW
::web::mc::CannotFindBrowserDetail	"Set the BROWSER environment variable to your desired browser." ;# NEW

### colormenu ##########################################################
::colormenu::mc::BaseColor			"Color base"
::colormenu::mc::UserColor			"Color del usuario"
::colormenu::mc::UsedColor			"Color utilizado"
::colormenu::mc::RecentColor		"Color reciente"
::colormenu::mc::Texture			"Textura"
::colormenu::mc::OpenColorDialog	"Abrir el di�logo de colores"
::colormenu::mc::EraseColor		"Borrar color"
::colormenu::mc::Close				"Cerrar"

### table ##############################################################
::table::mc::Ok							"&Aceptar"
::table::mc::Cancel						"&Cancelar"
::table::mc::Column						"Columna"
::table::mc::Table						"Tabla"
::table::mc::Configure					"Configurar"
::table::mc::Hide							"Ocultar"
::table::mc::ShowColumn					"Mostrar columna"
::table::mc::Foreground					"Primer plano"
::table::mc::Background					"Segundo plano"
::table::mc::DisabledForeground		"Eliminar primer plano"
::table::mc::SelectionForeground		"Elegir primer plano"
::table::mc::SelectionBackground		"Elegir segundo plano"
::table::mc::HighlightColor			"Resaltar segundo plano"
::table::mc::Stripes						"Listas"
::table::mc::MinWidth					"Ancho m�nimo"
::table::mc::MaxWidth					"Ancho m�ximo"
::table::mc::Separator					"Separador"
::table::mc::AutoStretchColumn		"Auto estirar columna"
::table::mc::FillColumn					"- Llenar columna -"
::table::mc::Preview						"Vista previa"
::table::mc::OptimizeColumn			"Optimizar el ancho de la columna"
::table::mc::OptimizeColumns			"Optimizar todas las columnas"
::table::mc::FitColumnWidth			"Ajustar el ancho de columna"
::table::mc::FitColumns					"Ajustar todas las columnas"
::table::mc::SqueezeColumns			"Comprimir todas las columnas"
::table::mc::AccelFitColumns			"Ctrl+,"
::table::mc::AccelOptimizeColumns	"Ctrl+."
::table::mc::AccelSqueezeColumns		"Ctrl+#"

### choosecolor ########################################################
::dialog::choosecolor::mc::Ok					"&Aceptar"
::dialog::choosecolor::mc::Cancel			"&Cancelar"

::dialog::choosecolor::mc::BaseColors		"Colores base"
::dialog::choosecolor::mc::UserColors		"Colores del usuario"
::dialog::choosecolor::mc::RecentColors	"Colores recientes"
::dialog::choosecolor::mc::OldColor			"Color antiguo"
::dialog::choosecolor::mc::CurrentColor	"Color actual"
::dialog::choosecolor::mc::Old				"Antiguo"
::dialog::choosecolor::mc::Current			"Actual"
::dialog::choosecolor::mc::Color				"Color"
::dialog::choosecolor::mc::ColorSelection	"Elecci�n del color"
::dialog::choosecolor::mc::Red				"Rojo"
::dialog::choosecolor::mc::Green				"Verde"
::dialog::choosecolor::mc::Blue				"Azul"
::dialog::choosecolor::mc::Hue				"Tono"
::dialog::choosecolor::mc::Saturation		"Saturaci�n"
::dialog::choosecolor::mc::Value				"Valor"
::dialog::choosecolor::mc::Enter				"Ingresar"
::dialog::choosecolor::mc::AddColor			"Agregar el color actual a los colores de usuario"
::dialog::choosecolor::mc::ClickToEnter	"Haga click para ingresar un valor hexadecimal"

### choosefont #########################################################
::dialog::choosefont::mc::Apply				"&Aplicar"
::dialog::choosefont::mc::Cancel				"&Cancelar"
::dialog::choosefont::mc::Continue			"Con&tinuar"
::dialog::choosefont::mc::FixedOnly			"S�lo fuentes m&onoespaciadas/fijas"
::dialog::choosefont::mc::Family				"Fam&ilia"
::dialog::choosefont::mc::Font				"&Fuente"
::dialog::choosefont::mc::Ok					"Ac&eptar"
::dialog::choosefont::mc::Reset				"&Reajustar"
::dialog::choosefont::mc::Size				"&Tama�o"
::dialog::choosefont::mc::Strikeout			"Tach&ado"
::dialog::choosefont::mc::Style				"Es&tilo"
::dialog::choosefont::mc::Underline			"S&ubrayado"
::dialog::choosefont::mc::Color				"Color"

::dialog::choosefont::mc::Effects			"Efectos"
::dialog::choosefont::mc::Filter				"Filtro"
::dialog::choosefont::mc::Sample				"Muestra"
::dialog::choosefont::mc::SearchTitle		"Buscando fuentes monoespaciadas/fijas"
::dialog::choosefont::mc::SeveralMinutes	"Esta operaci�n puede tomar cerca de %d minuto(s)."
::dialog::choosefont::mc::FontSelection	"Selecci�n de fuente"
::dialog::choosefont::mc::Wait				"Espere"

### choosedir ##########################################################
::choosedir::mc::FileSystem		"Sistema de archivos"
::choosedir::mc::ShowPredecessor	"Show Predecessor" ;# NEW
::choosedir::mc::ShowTail			"Show Tail" ;# NEW
::choosedir::mc::Folder				"Folder" ;# NEW

### fsbox ##############################################################
::fsbox::mc::Name								"Nombre"
::fsbox::mc::Size								"Tama�o"
::fsbox::mc::Modified						"Modified" ;# NEW

::fsbox::mc::Forward							"Forward to '%s'" ;# NEW
::fsbox::mc::Backward						"Backward to '%s'" ;# NEW
::fsbox::mc::Delete							"Eliminar"
::fsbox::mc::Rename							"Rename" ;# NEW
::fsbox::mc::NewFolder						"New Folder" ;# NEW
::fsbox::mc::Layout							"Disposici�n"
::fsbox::mc::ListLayout						"List Layout" ;# NEW
::fsbox::mc::DetailedLayout				"Detailed Layout" ;# NEW
::fsbox::mc::ShowHiddenDirs				"&Mostrar directorios ocultos"
::fsbox::mc::ShowHiddenFiles				"&Mostrar archivos y directorios ocultos"
::fsbox::mc::AppendToExisitingFile		"&Agregar partidas a un archivo existente"
::fsbox::mc::Cancel							"&Cancelar"
::fsbox::mc::Save								"&Guardar"
::fsbox::mc::Open								"&Abrir"

::fsbox::mc::AddBookmark					"Add Bookmark '%s'" ;# NEW
::fsbox::mc::RemoveBookmark				"Remove Bookmark '%s'" ;# NEW

::fsbox::mc::Filename						"&Nombre del archivo:"
::fsbox::mc::Filenames						"&Nombres del archivo:"
::fsbox::mc::FilesType						"&Tipo de archivos:"
::fsbox::mc::FileEncoding					"File &encoding:" ;# NEW

::fsbox::mc::Favorites						"Favorites" ;# NEW
::fsbox::mc::LastVisited					"Last Visited" ;# NEW
::fsbox::mc::FileSystem						"Sistema de archivos"
::fsbox::mc::Desktop							"Escritorio"
::fsbox::mc::Home								"Home" ;# NEW

::fsbox::mc::SelectWhichType				"Elegir qu� tipo de archivo mostrar"
::fsbox::mc::TimeFormat						"%d/%m/%y %I:%M %p" ;# NEW

::fsbox::mc::CannotChangeDir				"No se puede cambiar al directorio '%s'.\nPermiso denegado."
::fsbox::mc::DirectoryRemoved				"No se puede cambiar al directorio '%s'.\nEl directorio fue eliminado."
::fsbox::mc::ReallyMove(file,w)			"Really move file '%s' to trash?" ;# NEW
::fsbox::mc::ReallyMove(file,r)			"Really move write-protected file '%s' to trash?" ;# NEW
::fsbox::mc::ReallyMove(folder,w)		"Really move folder '%s' to trash?" ;# NEW
::fsbox::mc::ReallyMove(folder,r)		"Really move write-protected folder '%s' to trash?" ;# NEW
::fsbox::mc::ReallyDelete(file,w)		"Really delete file '%s'? You cannot undo this operation." ;# NEW
::fsbox::mc::ReallyDelete(file,r)		"Really delete write-protected file '%s'? You cannot undo this operation." ;# NEW
::fsbox::mc::ReallyDelete(link,w)		"Really delete link to '%s'?" ;# NEW
::fsbox::mc::ReallyDelete(link,r)		"Really delete link to '%s'?" ;# NEW
::fsbox::mc::ReallyDelete(folder,w)		"Really delete folder '%s'? You cannot undo this operation." ;# NEW
::fsbox::mc::ReallyDelete(folder,r)		"Really delete write-protected folder '%s'? You cannot undo this operation." ;# NEW
::fsbox::mc::DeleteFailed					"Deletion of '%s' failed." ;# NEW
::fsbox::mc::CommandFailed					"Command '%s' failed." ;# NEW
::fsbox::mc::ErrorRenaming(folder)		"Error renaming folder '%old' to '%new': permission denied." ;# NEW
::fsbox::mc::ErrorRenaming(file)			"Error renaming file '%old' to '%new': permission denied." ;# NEW
::fsbox::mc::InvalidFileExt				"Cannot rename because '%s' has an invalid file extension." ;# NEW
::fsbox::mc::CannotRename					"Cannot rename to '%s' because this folder/file already exists." ;# NEW
::fsbox::mc::CannotCreate					"Cannot create folder '%s' because this folder/file already exists." ;# NEW
::fsbox::mc::ErrorCreate					"Error creating folder: permission denied." ;# NEW
::fsbox::mc::FilenameNotAllowed			"Filename '%s' is not allowed." ;# NEW
::fsbox::mc::ContainsTwoDots				"Contains two consecutive dots." ;# NEW
::fsbox::mc::InvalidFileExtension		"Invalid file extension in '%s'." ;# NEW
::fsbox::mc::MissingFileExtension		"Missing file extension in '%s'." ;# NEW
::fsbox::mc::FileAlreadyExists			"El archivo '%s' ya existe.\n�Quiere sobreescribirlo?"
::fsbox::mc::CannotOverwriteDirectory	"Cannot overwite directory '%s'." ;# NEW
::fsbox::mc::FileDoesNotExist				"El archivo '%s' no existe."
::fsbox::mc::DirectoryDoesNotExist		"Directory '%s' does not exist." ;# NEW
::fsbox::mc::CannotOpenOrCreate			"Cannot open/create '%s'. Please choose a directory." ;# NEW

### toolbar ############################################################
::toolbar::mc::Toolbar		"Barra de herramientas"
::toolbar::mc::Orientation	"Orientaci�n"
::toolbar::mc::Alignment	"Alineaci�n"
::toolbar::mc::IconSize		"Tama�o de �conos"

::toolbar::mc::Default		"Predeterminado"
::toolbar::mc::Small			"Peque�o"
::toolbar::mc::Medium		"Medio"
::toolbar::mc::Large			"Grande"

::toolbar::mc::Top			"Arriba"
::toolbar::mc::Bottom		"Abajo"
::toolbar::mc::Left			"Izquierda"
::toolbar::mc::Right			"Derecha"
::toolbar::mc::Center		"Centro"

::toolbar::mc::Flat			"Plano"
::toolbar::mc::Floating		"Flotante"
::toolbar::mc::Hide			"Oculto"

::toolbar::mc::Expand		"Expandir"

### Countries ##########################################################
::country::mc::Afghanistan											"Afganist�n"
::country::mc::Netherlands_Antilles								"Antillas Holandesas"
::country::mc::Anguilla												"Anguila"
::country::mc::Aboard_Aircraft									"A bordo de un avi�n"
::country::mc::Aaland_Islands										"Islas Aaland"
::country::mc::Albania												"Albania"
::country::mc::Algeria												"Argelia"
::country::mc::Andorra												"Andorra"
::country::mc::Angola												"Angola"
::country::mc::Antigua												"Antigua y Barbuda"
::country::mc::Australasia											"Australasia"
::country::mc::Argentina											"Argentina"
::country::mc::Armenia												"Armenia"
::country::mc::Aruba													"Aruba"
::country::mc::American_Samoa										"Samoa americana"
::country::mc::Antarctica											"Ant�rtida"
::country::mc::French_Southern_Territories					"Territorios franceses del sur"
::country::mc::Australia											"Australia"
::country::mc::Austria												"Austria"
::country::mc::Azerbaijan											"Azerbaijan"
::country::mc::Bahamas												"Bahamas"
::country::mc::Bangladesh											"Bangladesh"
::country::mc::Barbados												"Barbados"
::country::mc::Basque												"Vascongada"
::country::mc::Burundi												"Burundi"
::country::mc::Belgium												"B�lgica"
::country::mc::Benin													"Benin"
::country::mc::Bermuda												"Bermuda"
::country::mc::Bhutan												"Bhutan"
::country::mc::Bosnia_and_Herzegovina							"Bosnia y Herzegovina"
::country::mc::Belize												"Belice"
::country::mc::Belarus												"Bielorrusia"
::country::mc::Bolivia												"Bolivia"
::country::mc::Brazil												"Brasil"
::country::mc::Bahrain												"Bahrein"
::country::mc::Brunei												"Brunei"
::country::mc::Botswana												"Botswana"
::country::mc::Bulgaria												"Bulgaria"
::country::mc::Burkina_Faso										"Burkina Faso"
::country::mc::Bouvet_Islands										"Islas Bouvet"
::country::mc::Central_African_Republic						"Rep�blica Centroafricana"
::country::mc::Cambodia												"Camboya"
::country::mc::Canada												"Canad�"
::country::mc::Catalonia											"Catalu�a"
::country::mc::Cayman_Islands										"Islas Caim�n"
::country::mc::Cocos_Islands										"Islas Cocos"
::country::mc::Congo													"Congo (Brazzaville)"
::country::mc::Chad													"Chad"
::country::mc::Chile													"Chile"
::country::mc::China													"China"
::country::mc::Ivory_Coast											"Costa de Marfil"
::country::mc::Cameroon												"Camer�n"
::country::mc::DR_Congo												"RD del Congo"
::country::mc::Cook_Islands										"Islas Cook"
::country::mc::Colombia												"Colombia"
::country::mc::Comoros												"Comoras"
::country::mc::Cape_Verde											"Cabo Verde"
::country::mc::Costa_Rica											"Costa Rica"
::country::mc::Croatia												"Croacia"
::country::mc::Cuba													"Cuba"
::country::mc::Christmas_Island									"Islas Christmas"
::country::mc::Cyprus												"Chipre"
::country::mc::Czech_Republic										"Rep�blica Checa"
::country::mc::Denmark												"Dinamarca"
::country::mc::Djibouti												"Djibouti"
::country::mc::Dominica												"Dominica"
::country::mc::Dominican_Republic								"Rep�blica Dominicana"
::country::mc::Ecuador												"Ecuador"
::country::mc::Egypt													"Egipto"
::country::mc::England												"Inglaterra"
::country::mc::Eritrea												"Etiop�a"
::country::mc::El_Salvador											"El Salvador"
::country::mc::Western_Sahara										"Sahara Occidental"
::country::mc::Spain													"Espa�a"
::country::mc::Estonia												"Estonia"
::country::mc::Ethiopia												"Etiop�a"
::country::mc::Faroe_Islands										"Islas Faroe"
::country::mc::Fiji													"Fidji"
::country::mc::Finland												"Finlandia"
::country::mc::Falkland_Islands									"Islas Malvinas"
::country::mc::France												"Francia"
::country::mc::West_Germany										"Alemania Occidental"
::country::mc::Micronesia											"Micronesia"
::country::mc::Gabon													"Gab�n"
::country::mc::Gambia												"Gambia"
::country::mc::Great_Britain										"Gran Breta�a"
::country::mc::Guinea_Bissau										"Guinea-Bissau"
::country::mc::Gibraltar											"Gibraltar"
::country::mc::Guernsey												"Guernsey"
::country::mc::East_Germany										"Alemania Oriental"
::country::mc::Georgia												"Georgia"
::country::mc::Equatorial_Guinea									"Guinea Ecuatorial"
::country::mc::Germany												"Alemania"
::country::mc::Ghana													"Ghana"
::country::mc::Guadeloupe											"Guadalupe"
::country::mc::Greece												"Grecia"
::country::mc::Grenada												"Grenada"
::country::mc::Greenland											"Groenlandia"
::country::mc::Guatemala											"Guatemala"
::country::mc::French_Guiana										"Guayana Francesa"
::country::mc::Guinea												"Guinea"
::country::mc::Guam													"Guam"
::country::mc::Guyana												"Guyana"
::country::mc::Haiti													"Hait�"
::country::mc::Hong_Kong											"Hong Kong"
::country::mc::Heard_Island_and_McDonald_Islands			"Isla Heard e Islas McDonald"
::country::mc::Honduras												"Honduras"
::country::mc::Hungary												"Hungr�a"
::country::mc::Isle_of_Man											"Isla de Man"
::country::mc::Indonesia											"Indonesia"
::country::mc::India													"India"
::country::mc::British_Indian_Ocean_Territory				"Territorio Brit�nico del Oc�ano �ndico"
::country::mc::Iran													"Ir�n"
::country::mc::Ireland												"Irlanda"
::country::mc::Iraq													"Irak"
::country::mc::Iceland												"Islandia"
::country::mc::Israel												"Israel"
::country::mc::Italy													"Italia"
::country::mc::British_Virgin_Islands							"Islas V�rgenes Brit�nicas"
::country::mc::Jamaica												"Jamaica"
::country::mc::Jersey												"Jersey"
::country::mc::Jordan												"Jordania"
::country::mc::Japan													"Jap�n"
::country::mc::Kazakhstan											"Kazajst�n"
::country::mc::Kenya													"Kenia"
::country::mc::Kosovo												"Kosovo"
::country::mc::Kyrgyzstan											"Kirguizist�n"
::country::mc::Kiribati												"Kiribati"
::country::mc::South_Korea											"Corea del Sur"
::country::mc::Saudi_Arabia										"Arabia Saudita"
::country::mc::Kuwait												"Kuwait"
::country::mc::Laos													"Laos"
::country::mc::Latvia												"Letonia"
::country::mc::Libya													"Libia"
::country::mc::Liberia												"Liberia"
::country::mc::Saint_Lucia											"Santa Luc�a"
::country::mc::Lesotho												"Lesotho"
::country::mc::Lebanon												"L�bano"
::country::mc::Liechtenstein										"Liechtenstein"
::country::mc::Lithuania											"Lituania"
::country::mc::Luxembourg											"Luxemburgo"
::country::mc::Macao													"Macao"
::country::mc::Madagascar											"Madagascar"
::country::mc::Morocco												"Marruecos"
::country::mc::Malaysia												"Malasia"
::country::mc::Malawi												"Malawi"
::country::mc::Moldova												"Moldova"
::country::mc::Maldives												"Maldivas"
::country::mc::Mexico												"Mexico"
::country::mc::Mongolia												"Mongolia"
::country::mc::Marshall_Islands									"Islas Marshall"
::country::mc::Macedonia											"Macedonia"
::country::mc::Mali													"Mali"
::country::mc::Malta													"Malta"
::country::mc::Montenegro											"Montenegro"
::country::mc::Northern_Mariana_Islands						"Islas Marianas Septentrionales"
::country::mc::Monaco												"Monaco"
::country::mc::Mozambique											"Mozambique"
::country::mc::Mauritius											"Mauricio"
::country::mc::Montserrat											"Montserrat"
::country::mc::Mauritania											"Mauritania"
::country::mc::Martinique											"Martinica"
::country::mc::Myanmar												"Myanmar"
::country::mc::Mayotte												"Mayotte"
::country::mc::Namibia												"Namibia"
::country::mc::Nicaragua											"Nicaragua"
::country::mc::New_Caledonia										"Nueva Caledonia"
::country::mc::Netherlands											"Pa�ses Bajos"
::country::mc::Nepal													"Nepal"
::country::mc::The_Internet										"Internet"
::country::mc::Norfolk_Island										"Isla Norfolk"
::country::mc::Nigeria												"Nigeria"
::country::mc::Niger													"N�ger"
::country::mc::Northern_Ireland									"Irlanda del Norte"
::country::mc::Niue													"Niue"
::country::mc::Norway												"Noruega"
::country::mc::Nauru													"Nauru"
::country::mc::New_Zealand											"Nueva Zelanda"
::country::mc::Oman													"Om�n"
::country::mc::Pakistan												"Pakist�n"
::country::mc::Panama												"Panam�"
::country::mc::Paraguay												"Paraguay"
::country::mc::Pitcairn_Islands									"Islas Pitcairn"
::country::mc::Peru													"Per�"
::country::mc::Philippines											"Filipinas"
::country::mc::Palestine											"Palestina"
::country::mc::Palau													"Palau"
::country::mc::Papua_New_Guinea									"Papua Nueva Guinea"
::country::mc::Poland												"Polonia"
::country::mc::Portugal												"Portugal"
::country::mc::North_Korea											"Corea del Norte"
::country::mc::Puerto_Rico											"Puerto Rico"
::country::mc::French_Polynesia									"Polinesia Francesa"
::country::mc::Qatar													"Qatar"
::country::mc::Reunion												"Reuni�n"
::country::mc::Romania												"Rumania"
::country::mc::South_Africa										"Sud�frica"
::country::mc::Russia												"Rusia"
::country::mc::Rwanda												"Rwanda"
::country::mc::Samoa													"Samoa"
::country::mc::Serbia_and_Montenegro							"Serbia y Montenegro"
::country::mc::Scotland												"Escocia"
::country::mc::At_Sea												"En el mar"
::country::mc::Senegal												"Senegal"
::country::mc::Seychelles											"Seychelles"
::country::mc::South_Georgia_and_South_Sandwich_Islands	"Isla Georgia y Sandwich del Sur"
::country::mc::Saint_Helena										"Santa Helena"
::country::mc::Singapore											"Singapur"
::country::mc::Jan_Mayen_and_Svalbard							"Svalbard y Jan Mayen"
::country::mc::Saint_Kitts_and_Nevis							"Saint Kitts y Nevis"
::country::mc::Sierra_Leone										"Sierra Leona"
::country::mc::Slovenia												"Eslovenia"
::country::mc::San_Marino											"San Marino"
::country::mc::Solomon_Islands									"Islas Solomon"
::country::mc::Somalia												"Somal�a"
::country::mc::Aboard_Spacecraft									"A bordo del transbordador espacial"
::country::mc::Saint_Pierre_and_Miquelon						"Saint Pierre y Miquelon"
::country::mc::Serbia												"Serbia"
::country::mc::Sri_Lanka											"Sri Lanka"
::country::mc::Sao_Tome_and_Principe							"Sao Tome y Pr�ncipe"
::country::mc::Sudan													"Sud�n"
::country::mc::Switzerland											"Suiza"
::country::mc::Suriname												"Surinam"
::country::mc::Slovakia												"Eslovaquia"
::country::mc::Sweden												"Suecia"
::country::mc::Swaziland											"Swazilandia"
::country::mc::Syria													"Siria"
::country::mc::Tanzania												"Tanzania"
::country::mc::Turks_and_Caicos_Islands						"Islas Turks y Caicos"
::country::mc::Czechoslovakia										"Checoslovaquia"
::country::mc::Tonga													"Tonga"
::country::mc::Thailand												"Tailandia"
::country::mc::Tibet													"T�bet"
::country::mc::Tajikistan											"Tajikist�n"
::country::mc::Tokelau												"Tokelau"
::country::mc::Turkmenistan										"Turkmenist�n"
::country::mc::Timor_Leste											"Timor Leste"
::country::mc::Togo													"Togo"
::country::mc::Chinese_Taipei										"Taiw�n"
::country::mc::Trinidad_and_Tobago								"Trinidad y Tobago"
::country::mc::Tunisia												"T�nez"
::country::mc::Turkey												"Turqu�a"
::country::mc::Tuvalu												"Tuvalu"
::country::mc::United_Arab_Emirates								"Emiratos �rabes Unidos"
::country::mc::Uganda												"Uganda"
::country::mc::Ukraine												"Ucrania"
::country::mc::United_States_Minor_Outlying_Islands		"Islas Remotas Menores de los Estados Unidos"
::country::mc::Unknown												"(Desconocido)"
::country::mc::Soviet_Union										"Uni�n Sovi�tica"
::country::mc::Uruguay												"Uruguay"
::country::mc::United_States_of_America						"Estados Unidos de Am�rica"
::country::mc::Uzbekistan											"Uzbekist�n"
::country::mc::Vanuatu												"Vanuatu"
::country::mc::Vatican												"Vaticano"
::country::mc::Venezuela											"Venezuela"
::country::mc::Vietnam												"Vietnam"
::country::mc::Saint_Vincent_and_the_Grenadines				"Saint Vincent y las Granadinas"
::country::mc::US_Virgin_Islands									"Islas V�rgenes Norteamericanas"
::country::mc::Wallis_and_Futuna									"Wallis y Futuna"
::country::mc::Wales													"Gales"
::country::mc::Yemen													"Yemen"
::country::mc::Yugoslavia											"Yugoslavia"
::country::mc::Zambia												"Zambia"
::country::mc::Zanzibar												"Zanz�bar"
::country::mc::Zimbabwe												"Zimbabwe"
::country::mc::Mixed_Team											"Equipo mixto"

::country::mc::Africa_North										"�frica, Norte"
::country::mc::Africa_Sub_Saharan								"�frica, Sub-Sahariana"
::country::mc::America_Caribbean									"Am�rica, Caribe"
::country::mc::America_Central									"Am�rica Central"
::country::mc::America_North										"Am�rica, Norte"
::country::mc::America_South										"Am�rica, Sur"
::country::mc::Antarctic											"Ant�rtida"
::country::mc::Asia_East											"Asia, Este"
::country::mc::Asia_South_South_East							"Asia, Sud-Sudeste"
::country::mc::Asia_West_Central									"Asia, Oeste-Central"
::country::mc::Europe												"Europa"
::country::mc::Europe_East											"Europa, Este"
::country::mc::Oceania												"Ocean�a"
::country::mc::Stateless											"Sin estado"

### Languages ##########################################################
::encoding::mc::Lang(FI)	"Fide"
::encoding::mc::Lang(af)	"Afrikaans"
::encoding::mc::Lang(ar)	"�rabe"
::encoding::mc::Lang(ast)	"Leonese"
::encoding::mc::Lang(az)	"Azerbaijani"
::encoding::mc::Lang(bat)	"B�ltico"
::encoding::mc::Lang(be)	"Bielorruso"
::encoding::mc::Lang(bg)	"B�lgaro"
::encoding::mc::Lang(br)	"Bret�n"
::encoding::mc::Lang(bs)	"Bosnio"
::encoding::mc::Lang(ca)	"Catal�n"
::encoding::mc::Lang(cs)	"Checo"
::encoding::mc::Lang(cy)	"Gal�s"
::encoding::mc::Lang(da)	"Dan�s"
::encoding::mc::Lang(de)	"Alem�n"
::encoding::mc::Lang(de+)	"Deutsch (reformed)" ;# NEW
::encoding::mc::Lang(el)	"Griego"
::encoding::mc::Lang(en)	"Ingl�s"
::encoding::mc::Lang(eo)	"Esperanto"
::encoding::mc::Lang(es)	"Espa�ol"
::encoding::mc::Lang(et)	"Estonio"
::encoding::mc::Lang(eu)	"Vasco"
::encoding::mc::Lang(fi)	"Fin�s"
::encoding::mc::Lang(fo)	"Faro�s"
::encoding::mc::Lang(fr)	"Franc�s"
::encoding::mc::Lang(fy)	"Fris�n"
::encoding::mc::Lang(ga)	"Irland�s"
::encoding::mc::Lang(gd)	"Escoc�s"
::encoding::mc::Lang(gl)	"Gallego"
::encoding::mc::Lang(he)	"Hebreo"
::encoding::mc::Lang(hi)	"Hindi"
::encoding::mc::Lang(hr)	"Croata"
::encoding::mc::Lang(hu)	"H�ngaro"
::encoding::mc::Lang(hy)	"Armenio"
::encoding::mc::Lang(ia)	"Interlingua"
::encoding::mc::Lang(is)	"Island�s"
::encoding::mc::Lang(it)	"Italiano"
::encoding::mc::Lang(iu)	"Inuktitut"
::encoding::mc::Lang(ja)	"Japon�s"
::encoding::mc::Lang(ka)	"Georgiano"
::encoding::mc::Lang(kk)	"Kazakho"
::encoding::mc::Lang(kl)	"Groenland�s"
::encoding::mc::Lang(ko)	"Coreano"
::encoding::mc::Lang(ku)	"Kurdo"
::encoding::mc::Lang(ky)	"Kirghiz"
::encoding::mc::Lang(la)	"Latin"
::encoding::mc::Lang(lb)	"Luxemburgu�s"
::encoding::mc::Lang(lt)	"Lituano"
::encoding::mc::Lang(lv)	"Let�n"
::encoding::mc::Lang(mk)	"Macedonio"
::encoding::mc::Lang(mo)	"Moldavo"
::encoding::mc::Lang(ms)	"Malayo"
::encoding::mc::Lang(mt)	"Malt�s"
::encoding::mc::Lang(nl)	"Holand�s"
::encoding::mc::Lang(no)	"Noruego"
::encoding::mc::Lang(oc)	"Occitano"
::encoding::mc::Lang(pl)	"Polaco"
::encoding::mc::Lang(pt)	"Portugu�s"
::encoding::mc::Lang(rm)	"Roman�"
::encoding::mc::Lang(ro)	"Rumano"
::encoding::mc::Lang(ru)	"Ruso"
::encoding::mc::Lang(se)	"Sami"
::encoding::mc::Lang(sk)	"Eslocavo"
::encoding::mc::Lang(sl)	"Esloveno"
::encoding::mc::Lang(sq)	"Alban�s"
::encoding::mc::Lang(sr)	"Serbio"
::encoding::mc::Lang(sv)	"Sueco"
::encoding::mc::Lang(sw)	"Swahili"
::encoding::mc::Lang(tg)	"Tajik"
::encoding::mc::Lang(th)	"Thai"
::encoding::mc::Lang(tk)	"Turkmeno"
::encoding::mc::Lang(tl)	"Tagalog"
::encoding::mc::Lang(tr)	"Turco"
::encoding::mc::Lang(uk)	"Ucraniano"
::encoding::mc::Lang(uz)	"Uzbeko"
::encoding::mc::Lang(vi)	"Vietnamita"
::encoding::mc::Lang(wa)	"Walloon"
::encoding::mc::Lang(wen)	"Sorbian"
::encoding::mc::Lang(hsb)	"Upper Sorbian" ;# NEW
::encoding::mc::Lang(dsb)	"Lower Sorbian" ;# NEW
::encoding::mc::Lang(zh)	"Chino"

::encoding::mc::Font(hi)	"Devanagari"

### Calendar ###########################################################
::calendar::mc::OneMonthForward	"Avanzar un mes (Shift-Flecha Derecha)"
::calendar::mc::OneMonthBackward	"Retroceder un mes (Shift-Flecha Izquierda)"
::calendar::mc::OneYearForward	"Avanzar un a�o (Ctrl-Flecha Derecha)"
::calendar::mc::OneYearBackward	"Retroceder un a�o (Ctrl-Flecha Izquierda)"

::calendar::mc::Su	"Do"
::calendar::mc::Mo	"Lu"
::calendar::mc::Tu	"Ma"
::calendar::mc::We	"Mi"
::calendar::mc::Th	"Ju"
::calendar::mc::Fr	"Vi"
::calendar::mc::Sa	"Sa"

::calendar::mc::Jan	"Ene"
::calendar::mc::Feb	"Feb"
::calendar::mc::Mar	"Mar"
::calendar::mc::Apr	"Abr"
::calendar::mc::May	"May"
::calendar::mc::Jun	"Jun"
::calendar::mc::Jul	"Jul"
::calendar::mc::Aug	"Ago"
::calendar::mc::Sep	"Sep"
::calendar::mc::Oct	"Oct"
::calendar::mc::Nov	"Nov"
::calendar::mc::Dec	"Dic"

::calendar::mc::MonthName(1)		"Enero"
::calendar::mc::MonthName(2)		"Febrero"
::calendar::mc::MonthName(3)		"Marzo"
::calendar::mc::MonthName(4)		"Abril"
::calendar::mc::MonthName(5)		"Mayo"
::calendar::mc::MonthName(6)		"Junio"
::calendar::mc::MonthName(7)		"Julio"
::calendar::mc::MonthName(8)		"Agosto"
::calendar::mc::MonthName(9)		"Septiembre"
::calendar::mc::MonthName(10)		"Octubre"
::calendar::mc::MonthName(11)		"Noviembre"
::calendar::mc::MonthName(12)		"Diciembre"

::calendar::mc::WeekdayName(0)	"Domingo"
::calendar::mc::WeekdayName(1)	"Lunes"
::calendar::mc::WeekdayName(2)	"Martes"
::calendar::mc::WeekdayName(3)	"Mi�rcoles"
::calendar::mc::WeekdayName(4)	"Jueves"
::calendar::mc::WeekdayName(5)	"Viernes"
::calendar::mc::WeekdayName(6)	"S�bado"

### remote #############################################################
::remote::mc::PostponedMessage "La apertura de la base \"%s\" se pospondr� hasta que concluya la operaci�n en curso."

# vi:set ts=3 sw=3:
