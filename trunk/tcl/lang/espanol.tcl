# ======================================================================
# Author : $Author$
# Version: $Revision: 420 $
# Date   : $Date: 2012-09-09 14:33:43 +0000 (Sun, 09 Sep 2012) $
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
# Copyright: (C) 2011-2012 Gregor Cramer
# Copyright: (C) 2011 Carlos Fernando Gonzalez
# Copyright: (C) 2012 Juan Carlos Vasquez R.
# ======================================================================

# ======================================================================
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# ======================================================================

# ======================================================================
# File encoding: utf-8
# ======================================================================

### global #############################################################
::mc::SortMapping		{á a Á A é e É E í i Í I ñ n Ñ N ó o Ó O ú u Ú U}
::mc::AsciiMapping	{á a Á A é e É E í i Í I ñ n Ñ N ó o Ó O ú u Ú U}
::mc::SortOrder		{A Á B C D E É F G H I Í J K L M N Ñ O Ó P Q R S T U Ú V W X Y Z a á b c d e é f g h i í j k l m n ñ o ó p q r s t u ú v w x y z}

::mc::Key(Alt)			"Alt" 
::mc::Key(Ctrl)		"Ctrl" 
::mc::Key(Down)		"\u2193"
::mc::Key(End)			"Fin"
::mc::Key(Home)		"Inicio"
::mc::Key(Left)		"\u2190"
::mc::Key(Next)		"Página\u2193"
::mc::Key(Prior)		"Página\u2191"
::mc::Key(Right)		"\u2192"
::mc::Key(Shift)		"Mayúsculas" 
::mc::Key(Up)			"\u2191"

::mc::Alignment		"Alineación"
::mc::Apply				"Aplicar"
::mc::Archive			"Archivo" 
::mc::Background		"Fondo"
::mc::Black				"Negras"
::mc::Bottom			"Inferior"
::mc::Cancel			"Cancelar"
::mc::Clear				"Vaciar"
::mc::Close				"Cerrar"
::mc::Color				"Color"
::mc::Colors			"Colores"
::mc::Configuration	"Configuration" ;# NEW
::mc::Copy				"Copiar"
::mc::Cut				"Cortar"
::mc::Dark				"Oscuras"
::mc::Database			"Base"
::mc::Delete			"Eliminar"
::mc::Edit				"Editar"
::mc::Escape			"Esc"
::mc::File				"File" ;# NEW
::mc::From				"De"
::mc::Game				"Partida"
::mc::Layout			"Disposición"
::mc::Left				"Izquierda"
::mc::Lite				"Claras"
::mc::Modify			"Modificar"
::mc::No					"no"
::mc::NotAvailable	"n/d"
::mc::Number			"Número"
::mc::OK					"Aceptar"
::mc::Order				"Orden"
::mc::Paste				"Pegar"
::mc::PieceSet			"Piezas"
::mc::Preview			"Vista previa"
::mc::Redo				"Deshacer"
::mc::Remove			"Quitar" 
::mc::Reset				"Restablecer"
::mc::Right				"Derecha"
::mc::SelectAll		"Seleccionar todo"
::mc::Texture			"Textura"
::mc::Theme				"Tema"
::mc::To					"A"
::mc::Top				"Superior"
::mc::Undo				"Deshacer"
::mc::Variation		"Variante"
::mc::White				"Blancas"
::mc::Yes				"Sí"

::mc::LogicalReset	"Restablecer"
::mc::LogicalAnd		"Y"
::mc::LogicalOr		"O"
::mc::LogicalNot		"No"

::mc::King				"Rey"
::mc::Queen				"Dama"
::mc::Rook				"Torre"
::mc::Bishop			"Alfil"
::mc::Knight			"Caballo"
::mc::Pawn				"Peón"

### scidb ##############################################################
::scidb::mc::CannotOverwriteTheme	"No se puede anular el tema %s."

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
::widget::mc::Next		"Pró&ximo"
::widget::mc::First		"Pr&imero" 
::widget::mc::Last		"Últi&mo" 
::widget::mc::Help		"Ayuda" 

::widget::mc::New			"&New" ;# NEW
::widget::mc::Save		"&Guardar"
::widget::mc::Delete		"&Eliminar"

::widget::mc::Control(minimize)	"Minimizar" 
::widget::mc::Control(restore)	"Salir de pantalla completa"
::widget::mc::Control(close)		"Cerrar"

### util ###############################################################

::util::mc::IOErrorOccurred					"Hubo un error de E/S"

::util::mc::IOError(OpenFailed)				"apertura fallida"
::util::mc::IOError(ReadOnly)					"la base es de sólo lectura"
::util::mc::IOError(UnknownVersion)			"versión desconocida del archivo"
::util::mc::IOError(UnexpectedVersion)		"versión no esperada del archivo"
::util::mc::IOError(Corrupted)				"archivo corrupto"
::util::mc::IOError(WriteFailed)				"operación de escritura fallida"
::util::mc::IOError(InvalidData)				"datos no válidos (archivo probablemente corrupto)" 
::util::mc::IOError(ReadError)				"error de lectura"
::util::mc::IOError(EncodingFailed)			"no se puede escribir el nombre" 
::util::mc::IOError(MaxFileSizeExceeded)	"se alcanzó el tamaño máximo de archivo" 
::util::mc::IOError(LoadFailed)				"carga fallida (demasiadas entradas de evento)" 

::util::mc::SelectionOwnerDidntRespond		"Tiempo excedido durante la operación: el propietario de la selección no respondió."

### progress ###########################################################
::progress::mc::Progress							"Progreso"

::progress::mc::Message(preload-namebase)		"Pre-cargando datos de jugadores"
::progress::mc::Message(preload-tournament)	"Pre-cargando datos de torneos"
::progress::mc::Message(preload-player)		"Pre-cargando datos de jugadores"
::progress::mc::Message(preload-annotator)	"Pre-cargando datos de anotadores"

::progress::mc::Message(read-index)				"Cargando úndices"
::progress::mc::Message(read-game)				"Cargando datos de partida"
::progress::mc::Message(read-namebase)			"Cargando nombres"
::progress::mc::Message(read-tournament)		"Cargando datos de torneos"
::progress::mc::Message(read-player)			"Cargando datos de jugadores"
::progress::mc::Message(read-annotator)		"Cargando datos de anotadores"
::progress::mc::Message(read-source)			"Cargando datos de fuentes"
::progress::mc::Message(read-team)				"Cargando datos de equipos"
::progress::mc::Message(read-init)				"Cargando datos de incialización"

::progress::mc::Message(write-index)			"Escribiendo indices"
::progress::mc::Message(write-game)				"Escribiendo datos de la partida"
::progress::mc::Message(write-namebase)		"Escribiendo datos de nombres"

### menu ###############################################################
::menu::mc::Theme							"Tema"

::menu::mc::AllScidbFiles				"Todos los archivos Scidb"
::menu::mc::AllScidbBases				"Todas las bases Scidb"
::menu::mc::ScidBases					"Bases Scid"
::menu::mc::ScidbBases					"Bases Scidb"
::menu::mc::ChessBaseBases				"Bases ChessBase"
::menu::mc::ScidbArchives				"Archivos Scidb"
::menu::mc::PGNFilesArchives			"Archivos PGN"
::menu::mc::PGNFiles						"Archivos PGN"
::menu::mc::PGNArchives					"Archivos PGN"

::menu::mc::Language						"&Idioma"
::menu::mc::Toolbars						"&Barras de herramientas"
::menu::mc::ShowLog						"Mostrar &bitácora"
::menu::mc::AboutScidb					"A&cerca de Scidb"
::menu::mc::Fullscreen					"&Pantalla completa"
::menu::mc::LeaveFullscreen			"Salir de &pantalla completa"
::menu::mc::Help							"&Ayuda"
::menu::mc::Contact						"&Contenidos (navegador web)"
::menu::mc::Quit							"&Salir"
::menu::mc::Extras						"E&xtras" 
::menu::mc::Setup							"Setu&p" ;# NEW
::menu::mc::Engines						"&Engines" ;# NEW

::menu::mc::ContactBugReport			"&Reporte de errores"
::menu::mc::ContactFeatureRequest	"&Solicitud de característica"
::menu::mc::InstallChessBaseFonts	"Instalar Fuentes de ChessBase" 
::menu::mc::OpenEngineLog				"Open &Engine Log" ;# NEW

::menu::mc::OpenFile						"Abrir un archivo Scidb"
::menu::mc::NewFile						"Crear un archivo Scidb"
::menu::mc::ImportFiles					"Importar archivos PGN"
::menu::mc::Archiving					"Archivando" 
::menu::mc::CreateArchive				"Crear Archivo" 
::menu::mc::BuildArchive				"Crear archivo %s" 
::menu::mc::Data							"%s datos" 

### load ###############################################################
::load::mc::SevereError				"Error severo al cargar archivo ECO" 
::load::mc::FileIsCorrupt			"El archivo %s está corrupto:"
::load::mc::ProgramAborting		"Abortando programa."

::load::mc::Loading					"Cargando %s"
::load::mc::StartupFinished		"Inicio del programa completado"
::load::mc::SystemEncoding			"La codificación del sistema es '%s'"

::load::mc::ReadingFile(options)	"Leer archivo de opciones"
::load::mc::ReadingFile(engines)	"Reading engines file" ;# NEW

::load::mc::ECOFile					"archivo ECO"
::load::mc::EngineFile				"archivo de motor"
::load::mc::SpellcheckFile			"archivo de revisión"
::load::mc::LocalizationFile		"archivo de localización"
::load::mc::RatingList				"listado de rating %s"
::load::mc::WikipediaLinks			"enlace a Wikipedia"
::load::mc::ChessgamesComLinks	"enlace a chessgames.com"
::load::mc::Cities					"ciudades"
::load::mc::PieceSet					"piezas"
::load::mc::Theme						"tema"
::load::mc::Icons						"íconos"

### archive ############################################################
::archive::mc::CorruptedArchive			"Archivo '%s' está corrupto."
::archive::mc::NotAnArchive				"'%s' no es un archivo."
::archive::mc::CorruptedHeader			"El encabezado del archivo en '%s' está corrupto."
::archive::mc::CannotCreateFile			"No se pudo crear e archivo '%s'."
::archive::mc::FailedToExtractFile		"No se pudo extraer el archivo '%s'."
::archive::mc::UnknownCompression		"Método de compresión desconocido '%s'." 
::archive::mc::ChecksumError				"Error de checksum al extraer '%s'." 
::archive::mc::ChecksumErrorDetail		"El archivo extraúdo '%s' se corromperá." 
::archive::mc::FileNotReadable			"El archivo '%s' no puede leerse." ;
::archive::mc::UsingRawInstead			"Usando método 'raw' de compresión." 
::archive::mc::CannotOpenArchive			"No se puede abrir el archivo '%s'." 
::archive::mc::CouldNotCreateArchive	"No se puede crear el archivo '%s'." ;

::archive::mc::PackFile						"Comprimir %s" ;
::archive::mc::UnpackFile					"Descomprimir %s" ;

### player photos ######################################################
::util::photos::mc::InstallPlayerPhotos		"Install/Update Player Photos" ;# NEW
::util::photos::mc::TimeOut						"Timeout occurred." ;# NEW
::util::photos::mc::EnterPassword				"Personal Password" ;# NEW
::util::photos::mc::Download						"Download" ;# NEW
::util::photos::mc::SharedInstallation			"Shared installation" ;# NEW
::util::photos::mc::LocalInstallation			"Private installation" ;# NEW
::util::photos::mc::RetryLater					"Please retry later." ;# NEW
::util::photos::mc::DownloadStillInProgress	"Download of photo files is still in progress." ;# NEW
::util::photos::mc::PhotoFiles					"Photo Files" ;# NEW

::util::photos::mc::RequiresSuperuserRights	"The installation/update requires super-user rights.\n\nNote that the password will not be accepted if your user is not in the sudoers file."
::util::photos::mc::RequiresInternetAccess	"The installation/update of the player photo files requires an internet connection." ;# NEW
::util::photos::mc::AlternativelyDownload(0)	"Alternatively you may download the photo files from %link%. Install these files into directory %local%." ;# NEW
::util::photos::mc::AlternativelyDownload(1)	"Alternatively you may download the photo files from %link%. Install these files into the shared directory %shared%, or into the private directory %local%." ;# NEW

::util::photos::mc::Error(nohttp)				"Cannot open an internet connection because package TclHttp is not installed." ;# NEW
::util::photos::mc::Error(busy)					"The installation/update is already running." ;# NEW
::util::photos::mc::Error(failed)				"Unexpected error: The invocation of the sub-process has failed." ;# NEW
::util::photos::mc::Error(passwd)				"The password is wrong." ;# NEW
::util::photos::mc::Error(nosudo)				"Cannot invoke 'sudo' command because your user is not in the sudoers file." ;# NEW
::util::photos::mc::Detail(nosudo)				"As a workaround you may do a private installation, or start this application as a super-user." ;# NEW

::util::photos::mc::Message(uptodate)			"The photo files are already up-to-date." ;# NEW
::util::photos::mc::Message(finished)			"The installation/update of photo files has finished." ;# NEW
::util::photos::mc::Message(broken)				"Broken Tcl library version." ;# NEW
::util::photos::mc::Message(noperm)				"You dont have write permissions for directory '%s'." ;# NEW
::util::photos::mc::Message(missing)			"Cannot find directory '%s'." ;# NEW
::util::photos::mc::Message(httperr)			"HTTP error: %s" ;# NEW
::util::photos::mc::Message(httpcode)			"Unexpected HTTP code %s." ;# NEW
::util::photos::mc::Message(noconnect)			"HTTP connection failed." ;# NEW
::util::photos::mc::Message(timeout)			"HTTP timeout occurred." ;# NEW
::util::photos::mc::Message(crcerror)			"Checksum error occurred. Possibly the file server is currently in maintenance mode." ;# NEW
::util::photos::mc::Message(maintenance)		"Photo file server maintenance is currently in progress." ;# NEW
::util::photos::mc::Message(notfound)			"Download aborted because photo file server maintenance is currently in progress." ;# NEW
::util::photos::mc::Message(aborted)			"User has aborted download." ;# NEW
::util::photos::mc::Message(killed)				"Unexpected termination of download. The sub-process has died." ;# NEW

::util::photos::mc::Detail(nohttp)				"Please install package TclHttp, for example %s." ;# NEW
::util::photos::mc::Detail(noconnect)			"Probably you don't have an internet connection." ;# NEW
::util::photos::mc::Detail(badhost)				"Another possibility is a bad host, or a bad port." ;# NEW

::util::photos::mc::Log(started)					"Installation/update of photo files started at %s." ;# NEW
::util::photos::mc::Log(finished)				"Installation/update of photo files finished at %s." ;# NEW
::util::photos::mc::Log(destination)			"Destination directory for photo file download is '%s'." ;# NEW
::util::photos::mc::Log(created)					"%s file(s) created." ;# NEW
::util::photos::mc::Log(deleted)					"%s file(s) deleted." ;# NEW
::util::photos::mc::Log(skipped)					"%s file(s) skipped." ;# NEW
::util::photos::mc::Log(updated)					"%s file(s) updated." ;# NEW

### application ########################################################
::application::mc::Database				"&Base"
::application::mc::Board					"&Tablero"
::application::mc::MainMenu				"&Menu principal"

::application::mc::DockWindow				"Ventana acoplada"
::application::mc::UndockWindow			"Ventana desacoplada"
::application::mc::ChessInfoDatabase	"Base de Datos Ajedrecística"
::application::mc::Shutdown				"Cierre..."
::application::mc::QuitAnyway				"Quit anyway?" ;# NEW

### application::board #################################################
::application::board::mc::ShowCrosstable		"Mostrar tabla de torneo para esta partida"

::application::board::mc::Tools					"Herramientas"
::application::board::mc::Control				"Control"
::application::board::mc::GoIntoNextVar		"Ir a la próxima variante"
::application::board::mc::GoIntPrevVar			"Ir a la variante previa"

::application::board::mc::Accel(edit-annotation)	"A"
::application::board::mc::Accel(edit-comment)		"C"
::application::board::mc::Accel(edit-marks)			"M"
::application::board::mc::Accel(add-new-game)		"S" 
::application::board::mc::Accel(replace-game)		"R" 
::application::board::mc::Accel(replace-moves)		"V" 
::application::board::mc::Accel(trial-mode)			"T" 

### application::database ##############################################
::application::database::mc::FileOpen						"Abrir Base..."
::application::database::mc::FileOpenRecent				"Abrir Recientes"
::application::database::mc::FileNew						"Nueva Base..."
::application::database::mc::FileExport					"Exportar..."
::application::database::mc::FileImport					"Importar archivos PGN..."
::application::database::mc::FileCreate					"Crear Archivo..."
::application::database::mc::FileClose						"Cerrar"
::application::database::mc::FileCompact					"Compactar"
::application::database::mc::HelpSwitcher					"Ayuda con el Cambiador de Bases de Datos"

::application::database::mc::Games							"&Partidas"
::application::database::mc::Players						"&Jugadores"
::application::database::mc::Events							"Even&tos"
::application::database::mc::Sites							"Lugare&s"
::application::database::mc::Annotators					"&Comentaristas"

::application::database::mc::File							"Archivo"
::application::database::mc::SymbolSize					"Tamaño del símbolo"
::application::database::mc::Large							"Grande"
::application::database::mc::Medium							"Mediano"
::application::database::mc::Small							"Pequeño"
::application::database::mc::Tiny							"Diminuto"
::application::database::mc::Empty							"vacío"
::application::database::mc::None							"ninguno"
::application::database::mc::Failed							"fallido"
::application::database::mc::LoadMessage					"Abrir Base %s"
::application::database::mc::UpgradeMessage				"Actualizando Database %s"
::application::database::mc::CompactMessage				"Compactando base de datos %s" 
::application::database::mc::CannotOpenFile				"No se puede abrir el archivo '%s'."
::application::database::mc::EncodingFailed				"Fallo en la codificación de %s."
::application::database::mc::DatabaseAlreadyOpen		"La Base '%s' ya está abierta."
::application::database::mc::Properties					"Propiedades"
::application::database::mc::Preload						"Precarga"
::application::database::mc::MissingEncoding				"Codificación %s perdida (usar %s en su lugar)"
::application::database::mc::DescriptionTooLarge		"Descripción demasiado grande."
::application::database::mc::DescrTooLargeDetail		"La entrada contiene %d caracteres, pero sólo se permiten %d."
::application::database::mc::ClipbaseDescription		"Base temporal, no se guarda al disco."
::application::database::mc::HardLinkDetected			"No se puede cargar el archivo '%file1' porque ya está cargado como '%file2'. Esto sucede cuando se usan hard links."
::application::database::mc::HardLinkDetectedDetail	 "Si se carga la misma base de datos nuevamente, la aplicació puede terminar debido a los hilos usados."
::application::database::mc::UriRejectedDetail			"Solamente pueden abrirse bases de datos Scidb:"
::application::database::mc::EmptyUriList					"Descartar contenido está vacóo."
::application::database::mc::OverwriteExistingFiles	"Sobrescribir archivos existentes en el directorio '%s'?"
::application::database::mc::SelectDatabases				"Seleccione las bases de datos que se abrirán"
::application::database::mc::ExtractArchive				"Extracer archivo %s"
::application::database::mc::CompactDetail				"Todos los juegos deben cerrarse para poder compactar."
::application::database::mc::ReallyCompact				"¿Realmente desea compactar la base de datos '%s'?" 
::application::database::mc::ReallyCompactDetail(1)	"Solamente se borrará una partida." 
::application::database::mc::ReallyCompactDetail(N)	"Se borrarán %s partidas."

::application::database::mc::RecodingDatabase			"Recodificar %base de %from a %to"
::application::database::mc::RecodedGames					"%s partida(s) recodificadas"

::application::database::mc::GameCount						"Partidas"
::application::database::mc::DatabasePath					"ruta a la Base"
::application::database::mc::DeletedGames					"Partidas eliminadas"
::application::database::mc::Description					"Descripción"
::application::database::mc::Created						"Creada"
::application::database::mc::LastModified					"Última modificación"
::application::database::mc::Encoding						"Codificar"
::application::database::mc::YearRange						"Rango de años"
::application::database::mc::RatingRange					"Rango de ratings"
::application::database::mc::Result							"Resultado"
::application::database::mc::Score							"puntuación"
::application::database::mc::Type							"Tipo"
::application::database::mc::ReadOnly						"Sólo lectura"

::application::database::mc::ChangeIcon					"Cambiar ícono"
::application::database::mc::Recode							"Recodificar"
::application::database::mc::EditDescription				"Editar Descripción"
::application::database::mc::EmptyClipbase				"Vaciar Base temporal"

::application::database::mc::T_Unspecific					"Inespecífico"
::application::database::mc::T_Temporary					"Temporal"
::application::database::mc::T_Work							"Trabajo"
::application::database::mc::T_Clipbase					"Base temporal"
::application::database::mc::T_MyGames						"Mis partidas"
::application::database::mc::T_Informant					"Informador"
::application::database::mc::T_LargeDatabase				"Gran Base"
::application::database::mc::T_CorrespondenceChess		"Ajedrez por Correspondencia"  
::application::database::mc::T_EmailChess					"Ajedrez por email"
::application::database::mc::T_InternetChess				"Ajedrez por Internet"
::application::database::mc::T_ComputerChess				"Ajedrez por computadora"
::application::database::mc::T_Chess960					"Ajedrez 960"
::application::database::mc::T_PlayerCollection			"Colección de jugadores"
::application::database::mc::T_Tournament					"Torneo"
::application::database::mc::T_TournamentSwiss			"Torneo suizo"
::application::database::mc::T_GMGames						"Partidas de GM"
::application::database::mc::T_IMGames						"Partidas de MI"
::application::database::mc::T_BlitzGames					"Partidas rápidas"
::application::database::mc::T_Tactics						"Táctica"
::application::database::mc::T_Endgames					"Finales"
::application::database::mc::T_Analysis					"Análisis"
::application::database::mc::T_Training					"Entrenamiento"
::application::database::mc::T_Match						"Competencia"
::application::database::mc::T_Studies						"Estudios"
::application::database::mc::T_Jewels						"Joyas"
::application::database::mc::T_Problems					"Problemas"
::application::database::mc::T_Patzer						"Novato"
::application::database::mc::T_Gambit						"Gambito"
::application::database::mc::T_Important					"Importante"
::application::database::mc::T_Openings					"Aperturas"
::application::database::mc::T_OpeningsWhite				"Aperturas de las Blancas"
::application::database::mc::T_OpeningsBlack				"Aperturas de las Negras"

::application::database::mc::OpenDatabase					"Abrir Base"
::application::database::mc::NewDatabase					"Nueva Base"
::application::database::mc::CloseDatabase				"Cerrar Base '%s'"
::application::database::mc::SetReadonly					"Marcar Base de Datos '%s' como solo lectura"
::application::database::mc::SetWriteable					"Marcar Base de Datos '%s' para escritura"

::application::database::mc::OpenReadonly					"Abrir como solo lectura"
::application::database::mc::OpenWriteable				"Abrir con permiso de escritura"

::application::database::mc::UpgradeDatabase				"%s es un formato antiguo de base de datos que solo puede abrirse como solo lectura.\n\nActualizarla creará una nueva versión de la base de datos y luego removerá los archivos originales.\n\nEsto puede demorar un poco pero solo debe hacerse una vez.\n\nDesea actualizar esta base de datos ahora?"
::application::database::mc::UpgradeDatabaseDetail		"\"No\" abrirá la base de datos como solo lectura y no puede marcarse para escritura."

### application::database::games #######################################
::application::database::games::mc::Control						"Control"
::application::database::games::mc::GameNumber					"Número de partida"

::application::database::games::mc::GotoFirstPage				"Ir a la primera página de partidas"
::application::database::games::mc::GotoLastPage				"Ir a la última página de partidas"
::application::database::games::mc::PreviousPage				"Página de partidas previa"
::application::database::games::mc::NextPage						"Próxima página de partidas"
::application::database::games::mc::GotoCurrentSelection		"Ir a la selección actual"
::application::database::games::mc::UseVerticalScrollbar		"Usar barra de deslizamiento vertical"
::application::database::games::mc::UseHorizontalScrollbar	"Usar barra de deslizamiento horizontal"
::application::database::games::mc::GotoEnteredGameNumber	"Ir al número de partida ingresado"

### application::database::players #####################################
::application::database::players::mc::EditPlayer				"Editar Jugador"
::application::database::players::mc::Score						"Puntuación"
::application::database::players::mc::TooltipRating			"Rating: %s"

### application::database::annotators ##################################
::application::database::annotators::mc::F_Annotator		"Comentarista"
::application::database::annotators::mc::F_Frequency		"Frecuencia"

::application::database::annotators::mc::Find				"Buscar"
::application::database::annotators::mc::FindAnnotator	"Buscar comentarista"
::application::database::annotators::mc::ClearEntries		"Vaciar entradas"
::application::database::annotators::mc::NotFound			"No se encontró."

### application::pgn ###################################################
::application::pgn::mc::Command(move:comment)			"Agregar comentario"
::application::pgn::mc::Command(move:marks)				"Agregar marcador"
::application::pgn::mc::Command(move:annotation)		"Agregar Nota/Comentario/Marcador"
::application::pgn::mc::Command(move:append)				"Agregar jugada"
::application::pgn::mc::Command(move:nappend)			"Agregar jugadas"
::application::pgn::mc::Command(move:exchange)			"Cambiar jugada"
::application::pgn::mc::Command(variation:new)			"Agregar variante"
::application::pgn::mc::Command(variation:replace)		"Reemplazar jugadas"
::application::pgn::mc::Command(variation:truncate)	"Truncar variante"
::application::pgn::mc::Command(variation:first)		"Convertir en primera variante"
::application::pgn::mc::Command(variation:promote)		"Transformar variante en Línea principal"
::application::pgn::mc::Command(variation:remove)		"Eliminar variante"
::application::pgn::mc::Command(variation:mainline)	"Nueva Línea principal"
::application::pgn::mc::Command(variation:insert)		"Agregar jugadas"
::application::pgn::mc::Command(variation:exchange)	"Permutar jugadas"
::application::pgn::mc::Command(strip:moves)				"Jugadas desde el principio"
::application::pgn::mc::Command(strip:truncate)			"Jugadas hasta el final"
::application::pgn::mc::Command(strip:annotations)		"Notas"
::application::pgn::mc::Command(strip:info)				"Mover Información"
::application::pgn::mc::Command(strip:marks)				"Marcadores"
::application::pgn::mc::Command(strip:comments)			"Comentarios"
::application::pgn::mc::Command(strip:variations)		"Variantes"
::application::pgn::mc::Command(copy:comments)			"Copiar Comentarios"
::application::pgn::mc::Command(move:comments)			"Mover Comentarios"
::application::pgn::mc::Command(game:clear)				"Vaciar partida"
::application::pgn::mc::Command(game:transpose)			"Partida transpuesta"

::application::pgn::mc::StartTrialMode						"Iniciar el modo de prueba"
::application::pgn::mc::StopTrialMode						"Terminar el modo de prueba"
::application::pgn::mc::Strip									"Limpiar"
::application::pgn::mc::InsertDiagram						"Insertar diagrama"
::application::pgn::mc::InsertDiagramFromBlack			"Insertar diagrama desde la perspectiva de las Negras"
::application::pgn::mc::SuffixCommentaries				"Comentarios en los sufijos"
::application::pgn::mc::StripOriginalComments			"Remover comentarios orginales"

::application::pgn::mc::LanguageSelection					"Idiomas" ;# NEW change to "Language Selection"
::application::pgn::mc::MoveNotation						"Anotación de Jugadas"
::application::pgn::mc::CollapseVariations				"Contraer variantes"
::application::pgn::mc::ExpandVariations					"Expandir variantes"
::application::pgn::mc::EmptyGame							"Vaciar partida"

::application::pgn::mc::NumberOfMoves						"Número de medias jugadas (en la línea principal):"
::application::pgn::mc::InvalidInput						"Entrada no válida '%d'."
::application::pgn::mc::MustBeEven							"La entrada debe ser par."
::application::pgn::mc::MustBeOdd							"La entrada debe ser impar."
::application::pgn::mc::CannotOpenCursorFiles			"No se puede abrir el archivo: %s"
::application::pgn::mc::ReallyReplaceMoves				"¿Realmente desea reemplazar las jugadas de la partida actual?"
::application::pgn::mc::CurrentGameIsNotModified		"La partida actual no fue modificada."

::application::pgn::mc::EditAnnotation						"Editar nota"
::application::pgn::mc::EditMoveInformation				"Editar información de la jugada"
::application::pgn::mc::EditCommentBefore					"Editar comentario antes de la jugada"
::application::pgn::mc::EditCommentAfter					"Editar comentario tras la jugada"
::application::pgn::mc::EditPrecedingComment				"Editar el comentario precedente"
::application::pgn::mc::EditTrailingComment				"Editar último comentario" 
::application::pgn::mc::EditMarks							"Editar marcador"
::application::pgn::mc::Display								"Mostrar"
::application::pgn::mc::None									"ninguno"

### application::tree ##################################################

::application::tree::mc::Total								"Total"
::application::tree::mc::Control								"Control"
::application::tree::mc::ChooseReferenceBase				"Elegir base de referencia"
::application::tree::mc::ReferenceBaseSwitcher			"Selector de base de referencia"
::application::tree::mc::Numeric								"Numérico"
::application::tree::mc::Bar									"Barras"
::application::tree::mc::StartSearch						"Iniciar búsqueda"
::application::tree::mc::StopSearch							"Suspender búsqueda"
::application::tree::mc::UseExactMode						"Usar posición de búsqueda"
::application::tree::mc::UseFastMode						"Usar búsqueda acelerada"
::application::tree::mc::UseQuickMode						"Usar búsqueda rápida"
::application::tree::mc::AutomaticSearch					"Busqueda automática"
::application::tree::mc::LockReferenceBase				"Bloquear base de referencia"
::application::tree::mc::SwitchReferenceBase				"Cambiar base de datos de referencia"
::application::tree::mc::TransparentBar					"Barras transparentes"

::application::tree::mc::FromWhitesPerspective			"Desde el lado de las Blancas"
::application::tree::mc::FromBlacksPerspective			"Desde el lado de las Negras"
::application::tree::mc::FromSideToMovePerspective		"Desde el lado que mueve"
::application::tree::mc::FromWhitesPerspectiveTip		"Evaluar desde la perspectiva de las blancas"
::application::tree::mc::FromBlacksPerspectiveTip		"Evaluar desde la perspectiva de las negras"

::application::tree::mc::TooltipAverageRating			"Rating promedio (%s)"
::application::tree::mc::TooltipBestRating				"Mejor rating (%s)"

::application::tree::mc::F_Number							"#"
::application::tree::mc::F_Move								"Jugada"
::application::tree::mc::F_Eco								"ECO"
::application::tree::mc::F_Frequency						"Frecuencia"
::application::tree::mc::F_Ratio								"Proporción"
::application::tree::mc::F_Score								"Resultado"
::application::tree::mc::F_Draws								"Tablas"
::application::tree::mc::F_Performance						"Desempeño"
::application::tree::mc::F_AverageYear						"\u00f8 Año"
::application::tree::mc::F_LastYear							"Última partida jugada"
::application::tree::mc::F_BestPlayer						"Mejor jugador"
::application::tree::mc::F_FrequentPlayer					"Jugador más frecuente"

::application::tree::mc::T_Number							"Numeración"
::application::tree::mc::T_AverageYear						"Año promedio"
::application::tree::mc::T_FrequentPlayer					"Jugador más frecuente"

### board ##############################################################
::board::mc::CannotReadFile		"No se puede leer el archivo '%s':"
::board::mc::CannotFindFile		"No se encuentra el archivo '%s'"
::board::mc::FileWillBeIgnored	"Se ignorará '%s' (ID duplicado)"
::board::mc::IsCorrupt				"'%s' está dañado (estilo %s desconocido '%s')"

::board::mc::ThemeManagement		"Manejo de temas"
::board::mc::Setup					"Disposición"

::board::mc::Default					"Por defecto"
::board::mc::WorkingSet				"Conjunto usado"

### board::options #####################################################
::board::options::mc::Coordinates			"Coordenadas"
::board::options::mc::SolidColor				"Color sólido"
::board::options::mc::EditList				"Editar lista"
::board::options::mc::Embossed				"repujado"
::board::options::mc::Highlighting			"Resaltar"
::board::options::mc::Border					"Borde"
::board::options::mc::SaveWorkingSet		"Guardar el conjunto usado"
::board::options::mc::SelectedSquare		"Casilla elegida"
::board::options::mc::ShowBorder				"Mostrar borde"
::board::options::mc::ShowCoordinates		"Mostrar coordenadas"
::board::options::mc::ShowMaterialValues	"Mostrar material"
::board::options::mc::ShowMaterialBar		"Mostrar Barra de Matrial" 
::board::options::mc::ShowSideToMove		"Mostrar el lado que mueve"
::board::options::mc::ShowSuggestedMove	"Mostar jugada sugerida"
::board::options::mc::SuggestedMove			"Jugada sugerida"
::board::options::mc::Basic					"Básico"
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
::board::options::mc::IsWriteProtected		"El archivo '%s' está protegido contra escritura."
::board::options::mc::ConfirmDelete			"¿Está seguro de eliminar '%s'?"
::board::options::mc::NoPermission			"No se puede eliminar '%s'.\nPermiso denegado."
::board::options::mc::BoardSetup				"Disposición del tablero"
::board::options::mc::OpenTextureDialog	"Abrir diálogo de texturas"

::board::options::mc::YouCannotReverse		"No puede revertir esta acción. El archivo '%s' será removido físicamente."

::board::options::mc::CannotUsePieceWorkingSet "No se puede crear un nuevo tema con el estilo de piezas %s elegido.\n Primero debe guardar el nuevo estilo de pieza, o elegir otro."

::board::options::mc::CannotUseSquareWorkingSet "No se puede crear un nuevo tema con el estilo de casillas %s elegido.\n Primero debe guardar el nuevo estilo de casillas, o elegir otro."

### board::piece #######################################################
::board::piece::mc::Start						"Iniciar"
::board::piece::mc::Stop						"Parar"
::board::piece::mc::HorzOffset				"Impresión horizontal"
::board::piece::mc::VertOffset				"Impresión vertical"
::board::piece::mc::Gradient					"Gradiente"
::board::piece::mc::Fill						"Relleno"
::board::piece::mc::Stroke						"Trazo"
::board::piece::mc::Contour					"Contorno"
::board::piece::mc::WhiteShape				"Silueta blanca"
::board::piece::mc::PieceSelection			"Selección de piezas"
::board::piece::mc::BackgroundSelection	"Selección de fondo"
::board::piece::mc::Zoom						"Zoom"
::board::piece::mc::Shadow						"Sombra"
::board::piece::mc::Opacity					"Opacidad"
::board::piece::mc::ShadowDiffusion			"Difusión de la sombra"
::board::piece::mc::PieceStyleConf			"Configuración del estilo de pieza"
::board::piece::mc::Offset						"Impresión"
::board::piece::mc::Rotate						"Rotar"
::board::piece::mc::CloseDialog				"¿Cerrar el diálogo y descartar cambios?"
::board::piece::mc::OpenTextureDialog		"Abrir diálogo de textura"

### board::square ######################################################
::board::square::mc::SolidColor			"Color sólido"
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
::board::square::mc::SquareStyleConf	"Configuración del estilo de las casillas"
::board::square::mc::CloseDialog			"¿Cerrar el diálogo y descartar los cambios?"

### board::texture #####################################################
::board::texture::mc::PreselectedOnly "Sólo preseleccionados"

### pgn-setup ##########################################################
::pgn::setup::mc::Configure(editor)				"Customize Editor" ;# NEW
::pgn::setup::mc::Configure(browser)			"Customize Text Output" ;# NEW
::pgn::setup::mc::TakeOver(editor)				"Adopt settings from Game Browser" ;# NEW
::pgn::setup::mc::TakeOver(browser)				"Adopt settings from Game Editor" ;# NEW
::pgn::setup::mc::Pixel								"pixel" ;# NEW
::pgn::setup::mc::RevertSettings					"Revert to initial settings" ;# NEW
::pgn::setup::mc::ResetSettings					"Reset to factory settings" ;# NEW
::pgn::setup::mc::DiscardAllChanges				"Discard all applied changes?" ;# NEW

::pgn::setup::mc::Setup(Appearance)				"Appearance" ;# NEW
::pgn::setup::mc::Setup(Layout)					"Layout" ;# NEW
::pgn::setup::mc::Setup(Diagrams)				"Diagrams" ;# NEW
::pgn::setup::mc::Setup(MoveStyle)				"Move Style" ;# NEW

::pgn::setup::mc::Setup(Fonts)					"Fonts" ;# NEW
::pgn::setup::mc::Setup(font-and-size)			"Text font and size" ;# NEW
::pgn::setup::mc::Setup(figurine-font)			"Figurine (normal)" ;# NEW
::pgn::setup::mc::Setup(figurine-bold)			"Figurine (bold)" ;# NEW
::pgn::setup::mc::Setup(symbol-font)			"Symbols" ;# NEW

::pgn::setup::mc::Setup(Colors)					"Colors" ;# NEW
::pgn::setup::mc::Setup(Highlighting)			"Highlighting" ;# NEW
::pgn::setup::mc::Setup(start-position)		"Start Position" ;# NEW
::pgn::setup::mc::Setup(variations)				"Variations" ;# NEW
::pgn::setup::mc::Setup(numbering)				"Numbering" ;# NEW
::pgn::setup::mc::Setup(brackets)				"Brackets" ;# NEW
::pgn::setup::mc::Setup(illegal-move)			"Illegal Move" ;# NEW
::pgn::setup::mc::Setup(comments)				"Comments" ;# NEW
::pgn::setup::mc::Setup(annotation)				"Annotation" ;# NEW
::pgn::setup::mc::Setup(marks)					"Marks" ;# NEW
::pgn::setup::mc::Setup(move-info)				"Move Information" ;# NEW
::pgn::setup::mc::Setup(result)					"Result" ;# NEW
::pgn::setup::mc::Setup(current-move)			"Current Move" ;# NEW
::pgn::setup::mc::Setup(next-moves)				"Next Moves" ;# NEW
::pgn::setup::mc::Setup(empty-game)				"Empty Game" ;# NEW

::pgn::setup::mc::Setup(Hovers)					"Hovers" ;# NEW
::pgn::setup::mc::Setup(hover-move)				"Move" ;# NEW
::pgn::setup::mc::Setup(hover-comment)			"Comment" ;# NEW
::pgn::setup::mc::Setup(hover-move-info)		"Move Information" ;# NEW

::pgn::setup::mc::Section(ParLayout)			"Paragraph Layout" ;# NEW
::pgn::setup::mc::ParLayout(use-spacing)		"Usar espaciado en los párrafos"
::pgn::setup::mc::ParLayout(column-style)		"Estilo columna"
::pgn::setup::mc::ParLayout(tabstop-1)			"Indent for White Move" ;# NEW
::pgn::setup::mc::ParLayout(tabstop-2)			"Indent for Black Move" ;# NEW
::pgn::setup::mc::ParLayout(mainline-bold)	"Negrita para las jugadas de la Línea principal"

::pgn::setup::mc::Section(Variations)			"Variation Layout" ;# NEW
::pgn::setup::mc::Variations(width)				"Indent Width" ;# NEW
::pgn::setup::mc::Variations(level)				"Indent Level" ;# NEW

::pgn::setup::mc::Section(Display)				"Display" ;# NEW
::pgn::setup::mc::Display(numbering)			"Show Variation Numbering" ;# NEW
::pgn::setup::mc::Display(moveinfo)				"Show Move Information" ;# NEW

::pgn::setup::mc::Section(Diagrams)				"Diagrams" ;# NEW
::pgn::setup::mc::Diagrams(show)					"Mostrar diagramas"
::pgn::setup::mc::Diagrams(square-size)		"Square Size" ;# NEW
::pgn::setup::mc::Diagrams(indentation)		"Indent Width" ;# NEW

### engine #############################################################
::engine::mc::Information				"Information" ;# NEW
::engine::mc::Options					"Options" ;# NEW

::engine::mc::Name						"Name" ;# NEW
::engine::mc::Identifier				"Identifier" ;# NEW
::engine::mc::Author						"Author" ;# NEW
::engine::mc::Country					"Country" ;# NEW
::engine::mc::Rating						"Rating" ;# NEW
::engine::mc::Logo						"Logo" ;# NEW
::engine::mc::Protocol					"Protocol" ;# NEW
::engine::mc::Parameters				"Parameters" ;# NEW
::engine::mc::Command					"Command" ;# NEW
::engine::mc::Variants					"Variants" ;# NEW
::engine::mc::LastUsed					"Last used" ;# NEW
::engine::mc::Frequency					"Frequency" ;# NEW

::engine::mc::Variant(standard)		"Standard Chess" ;# NEW
::engine::mc::Variant(chess960)		"Chess 960" ;# NEW
::engine::mc::Variant(shuffle)		"Shuffle Chess" ;# NEW

::engine::mc::SetupEngines				"Setup Engines" ;# NEW
::engine::mc::ImageFiles				"Image files" ;# NEW
::engine::mc::SelectEngine				"Select Engine" ;# NEW
::engine::mc::SelectEngineLogo		"Select Engine Logo" ;# NEW
::engine::mc::Executables				"Executables" ;# NEW
::engine::mc::EngineLog					"Engine Log" ;# NEW
::engine::mc::Probing					"Probing" ;# NEW
::engine::mc::NeverUsed					"never used" ;# NEW
::engine::mc::OpenFsbox					"Open File Selection Dialog" ;# NEW
::engine::mc::DefaultValue				"Default value" ;# NEW

::engine::mc::ConfirmNewEngine		"Confirm new engine" ;# NEW
::engine::mc::EngineAlreadyExists	"An entry with this engine already exists." ;# NEW
::engine::mc::CopyFromEngine			"Make a copy of entry" ;# NEW
::engine::mc::CannotOpenProcess		"Cannot start process." ;# NEW
::engine::mc::DoesNotRespond			"This engine does not respond either to UCI nor to XBoard/WinBoard protocol." ;# NEW
::engine::mc::DiscardChanges			"The current item has changed.\n\nReally discard changes?" ;# NEW
::engine::mc::ReallyDelete				"Really delete engine '%s'?" ;# NEW
::engine::mc::EntryAlreadyExists		"An entry with name '%s' already exists." ;# NEW

### gametable ##########################################################
::gametable::mc::DeleteGame				"Marcar partida como eliminada"
::gametable::mc::UndeleteGame				"Recuperar esta partida"
::gametable::mc::EditGameFlags			"Editar insignias de la partida"
::gametable::mc::Custom						"Habitual"

::gametable::mc::Monochrome				"Monocromo"
::gametable::mc::Transparent				"Transparente"
::gametable::mc::Relief						"Relieve"
::gametable::mc::ShowIdn					"Mostrar número de posición en Chess 960"
::gametable::mc::Icons						"Iconos"
::gametable::mc::Abbreviations			"Abreviaturas"

::gametable::mc::SortAscending			"Clasificar (ascendente)"
::gametable::mc::SortDescending			"Clasificar (descendente)"
::gametable::mc::SortOnAverageElo		"Clasificar por Elo promedio (descendente)"
::gametable::mc::SortOnAverageRating	"Clasificar por rating promedio (descendente)"
::gametable::mc::SortOnDate				"Clasificar por fecha (descendente)"
::gametable::mc::SortOnNumber				"Clasificar por número de partida (ascendente)"
::gametable::mc::ReverseOrder				"Invertir el orden"
::gametable::mc::NoMoves					"Sin jugadas"
::gametable::mc::NoMoreMoves				"No hay mas jugadas"
::gametable::mc::WhiteRating				"Rating de las Blancas"
::gametable::mc::BlackRating				"Rating de las Negras"

::gametable::mc::Flags						"Insignias"
::gametable::mc::PGN_CountryCode			"Código PGN de país"
::gametable::mc::ISO_CountryCode			"Código ISO de país"
::gametable::mc::ExcludeElo				"Excluir Elo"
::gametable::mc::IncludePlayerType		"Incluir tipo de jugador"
::gametable::mc::ShowTournamentTable	"Tabla del torneo"

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
::gametable::mc::F_Termination			"Terminación"
::gametable::mc::F_EventMode				"Modo"
::gametable::mc::F_Eco						"ECO"
::gametable::mc::F_Flags					"Insignias"
::gametable::mc::F_Material				"Material"
::gametable::mc::F_Acv						"ACV"
::gametable::mc::F_Idn						"960"
::gametable::mc::F_Position				"Posición"
::gametable::mc::F_EventDate				"Fecha del Evento"
::gametable::mc::F_EventType				"Tipo de Ev."
::gametable::mc::F_Changed					"Modificado"
::gametable::mc::F_Promotion				"Promoción"
::gametable::mc::F_UnderPromo				"Sub-promoción"
::gametable::mc::F_StandardPos			"Posición estándar"
::gametable::mc::F_Chess960Pos			"9"
::gametable::mc::F_Opening					"Apertura"
::gametable::mc::F_Variation				"Variante"
::gametable::mc::F_Subvariation			"Subvariante"
::gametable::mc::F_Overview				"Visión general"
::gametable::mc::F_Key						"Código ECO interno"

::gametable::mc::T_Number					"Número"
::gametable::mc::T_Acv						"Notas / Comentarios / Variantes"
::gametable::mc::T_WhiteRatingType		"Tipo de valuación de las Blancas"
::gametable::mc::T_BlackRatingType		"Tipo de valuación de las Negras"
::gametable::mc::T_WhiteCountry			"Federación de las Blancas"
::gametable::mc::T_BlackCountry			"Federación de las Negras"
::gametable::mc::T_WhiteTitle				"Título de las Blancas"
::gametable::mc::T_BlackTitle				"Título de las Negras"
::gametable::mc::T_WhiteType				"Tipo de las Blancas"
::gametable::mc::T_BlackType				"Tipo de las Negras"
::gametable::mc::T_WhiteSex				"Sexo de las Blancas"
::gametable::mc::T_BlackSex				"Sexo de las Negras"
::gametable::mc::T_EventCountry			"País del Evento"
::gametable::mc::T_EventType				"Tipo de Evento"
::gametable::mc::T_Chess960Pos			"Posición en Chess 960"
::gametable::mc::T_Deleted					"Eliminado"
::gametable::mc::T_EngFlag					"Insignia de idioma inglés"
::gametable::mc::T_OthFlag					"Insignia de otro idioma"
::gametable::mc::T_Idn						"Número de posición en Chess 960"
::gametable::mc::T_Annotations			"Notas"
::gametable::mc::T_Comments				"Comentarios"
::gametable::mc::T_Variations				"Variantes"
::gametable::mc::T_TimeMode				"Control de tiempo"

::gametable::mc::P_Rating					"Puntaje de rating"
::gametable::mc::P_RatingType				"Tipo de rating"
::gametable::mc::P_Country					"País"
::gametable::mc::P_Title					"Título"
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
::gametable::mc::EventType(simul)		"Simultáneas"
::gametable::mc::EventType(schev)		"Scheveningen"

::gametable::mc::PlayerType(human)		"Humano"
::gametable::mc::PlayerType(program)	"Computadora"

::gametable::mc::GameFlags(w)				"Apertura de las Blancas"
::gametable::mc::GameFlags(b)				"Apertura de las Negras"
::gametable::mc::GameFlags(m)				"Medio juego"
::gametable::mc::GameFlags(e)				"Final"
::gametable::mc::GameFlags(N)				"Novedad"
::gametable::mc::GameFlags(p)				"Estructura de peones"
::gametable::mc::GameFlags(T)				"Táctica"
::gametable::mc::GameFlags(K)				"Ala de rey"
::gametable::mc::GameFlags(Q)				"Ala de dama"
::gametable::mc::GameFlags(!)				"Genialidad"
::gametable::mc::GameFlags(?)				"Error Decisivo"
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
::gametable::mc::GameFlags(t)				"Error táctico"
::gametable::mc::GameFlags(s)				"Error estratégico"
::gametable::mc::GameFlags(C)				"Entoque Ilegal"
::gametable::mc::GameFlags(I)				"Jugada Ilegal"

### playertable ########################################################
::playertable::mc::F_LastName					"Apellido"
::playertable::mc::F_FirstName				"Nombre"
::playertable::mc::F_FideID					"ID del archivo"
::playertable::mc::F_Title						"Título"
::playertable::mc::F_Frequency				"Frecuencia"

::playertable::mc::T_Federation				"Federación"
::playertable::mc::T_RatingType				"Tipo de rating"
::playertable::mc::T_Type						"Tipo"
::playertable::mc::T_Sex						"Sexo"
::playertable::mc::T_PlayerInfo				"Bandera"

::playertable::mc::Find							"Búsqueda"
::playertable::mc::StartSearch				"Iniciar búsqueda"
::playertable::mc::ClearEntries				"Vaciar entradas"
::playertable::mc::NotFound					"No se encontró."

::playertable::mc::Name							"Nombre"
::playertable::mc::HighestRating				"Mayor rating"
::playertable::mc::MostRecentRating			"Rating más reciente"
::playertable::mc::DateOfBirth				"Fecha de nacimiento"
::playertable::mc::DateOfDeath				"Fecha de fallecimiento"
::playertable::mc::FideID						"Fide ID"

::playertable::mc::ShowPlayerCard			"Mostrar Tarjeta del Jugador..." 

### eventtable #########################################################
::eventtable::mc::Attendance	"Asistencia"

### player-card ########################################################
::playercard::mc::PlayerCard					"Tarjeta del Jugador"
::playercard::mc::Latest						"Máas reciente"
::playercard::mc::Highest						"Más alto"
::playercard::mc::Minimal						"Minimo"
::playercard::mc::Maximal						"Maximo" 
::playercard::mc::Win							"Ganadas" 
::playercard::mc::Draw							"Tablas" 
::playercard::mc::Loss							"Pérdidas" 
::playercard::mc::Total							"Total" 
::playercard::mc::FirstGamePlayed			"Primera partida jugada" 
::playercard::mc::LastGamePlayed				"Última partida jugada"
::playercard::mc::WhiteMostPlayed			"Aperturas más comunes con blancas"
::playercard::mc::BlackMostPlayed			"Aperturas más comunes con negras"

::playercard::mc::OpenInWebBrowser			"Abrir en un navegador"
::playercard::mc::OpenPlayerCard				"Abrir la tarjeta del jugador %s"
::playercard::mc::OpenFileCard				"Abrir la tarjeta del archivo %s"
::playercard::mc::OpenFideRatingHistory	"Abrir el historial de ratings de la Fide"
::playercard::mc::OpenWikipedia				"Abrir la biografía de Wikipedia"
::playercard::mc::OpenViafCatalog			"Abrir el catálogo VIAF"
::playercard::mc::OpenPndCatalog				"Abrir el catálogo de la Deutsche Nationalbibliothek"
::playercard::mc::OpenChessgames				"Colección de partidas de chessgames.com"
::playercard::mc::SeachIn365ChessCom		"Buscar en 365Chess.com"

### twm - tiled window manager #########################################
::twm::mc::Undock	"Desbloquear"
::twm::mc::Close	"Cerrar"

### fonts ##############################################################
::font::mc::ChessBaseFontsInstalled				"Las fuentes de ChessBase se instalaron correctamente."
::font::mc::ChessBaseFontsInstallationFailed	"Falló la instalación de las fuentes de ChessBase."
::font::mc::NoChessBaseFontFound					"No se encontraron las fuentes de ChessBase en la carpeta '%s'." 
::font::mc::ChessBaseFontsAlreadyInstalled	"Las fuentes de ChessBase ya se encuentran instaladas. ¿Instalar nuevamente?"
::font::mc::ChooseMountPoint						"Primero debe escoger el punto de montaje de la partición que contiene la instalació de Windows."
::font::mc::CopyingChessBaseFonts				"Copiando fuentes de ChessBase"
::font::mc::CopyFile									"Copiar archivo %s"
::font::mc::UpdateFontCache						"Actualizando caché de fuentes"

::font::mc::ChooseFigurineFont					"Choose figurine font" ;# NEW
::font::mc::ChooseSymbolFont						"Choose symbol font" ;# NEW
::font::mc::IncreaseFontSize						"Increase Font Size" ;# NEW
::font::mc::DecreaseFontSize						"Decrease Font Size" ;# NEW

### gamebar ############################################################
::gamebar::mc::StartPosition			"Iniciar posición"
::gamebar::mc::Players					"Jugadores"
::gamebar::mc::Event						"Evento"
::gamebar::mc::Site						"Lugar"
::gamebar::mc::SeparateHeader			"Encabezado separado"
::gamebar::mc::ShowActiveAtBottom	"Mostrar la partida activa al pie"
::gamebar::mc::ShowPlayersOnSeparateLines	"Mostrar los jugadores en líneas separadas"
::gamebar::mc::DiscardChanges			"Esta partida ha cambiado.\n\n¿Quiere realmente descartar los cambios realizados?"
::gamebar::mc::DiscardNewGame			"¿Realmente quiere descartar esta partida?"
::gamebar::mc::NewGameFstPart			"Nueva"
::gamebar::mc::NewGameSndPart			"Partida"

::gamebar::mc::LockGame					"Bloquear Juego"
::gamebar::mc::UnlockGame				"Desbloquear Juego"
::gamebar::mc::CloseGame				"Cerrar Juego"

::gamebar::mc::GameNew					"Nueva partida"
::gamebar::mc::GameNewChess960		"Nueva partida: Ajedrez 960"
::gamebar::mc::GameNewChess960Sym	"Nueva partida: Ajedrez 960 (sólo simétrico)"
::gamebar::mc::GameNewShuffle			"Nueva partida: Ajedrez Shuffle"

::gamebar::mc::AddNewGame				"Guardar: Agregar nueva partida a %s..."
::gamebar::mc::ReplaceGame				"Guardar: Reemplazar partida en %s..."
::gamebar::mc::ReplaceMoves			"Guardar: Reemplazar jugadas sólo en la partida"

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
::browser::mc::MaximizeBoardSize	"Maximizar el tamaño del tablero"
::browser::mc::MinimizeBoardSize	"Minimizar el tamaño del tablero"
::browser::mc::GotoFirstGame		"Ir a la primera partida"
::browser::mc::GotoLastGame		"Ir a la última partida"

::browser::mc::LoadGame				"Cargar partida"
::browser::mc::MergeGame			"Fusionar partida"

::browser::mc::IllegalMove			"Jugada ilegal"
::browser::mc::NoCastlingRights	"no puede enrocar"

### overview ###########################################################
::overview::mc::Overview				"Visión general"
::overview::mc::RotateBoard			"Girar el tablero"
::overview::mc::AcceleratorRotate	"R"

### encoding ###########################################################
::encoding::mc::AutoDetect				"auto-detection"

::encoding::mc::Encoding				"Codificar"
::encoding::mc::Description			"Descripción"
::encoding::mc::Languages				"Idiomas (Fuentes)"
::encoding::mc::UseAutoDetection		"Use Auto-Detection"

::encoding::mc::ChooseEncodingTitle	"Elegir Código"

::encoding::mc::CurrentEncoding		"Codificación actual:"
::encoding::mc::DefaultEncoding		"Codificación predeterminada:"
::encoding::mc::SystemEncoding		"Codificación del sistema:"

### setup ##############################################################
::setup::mc::Chess960Position			"Posición de Chess 960"
::setup::mc::SymmChess960Position	"Posición simétrica de chess 960"
::setup::mc::ShuffleChessPosition	"Posición de ajedrez Shuffle"

### setup board ########################################################
::setup::position::mc::SetStartPosition		"Configurar una posición inicial"
::setup::position::mc::UsePreviousPosition	"Usar una posición previa"

::setup::board::mc::SetStartBoard				"Configurar tablero de inicio"
::setup::board::mc::SideToMove					"Lado que mueve"
::setup::board::mc::Castling						"Enroque"
::setup::board::mc::MoveNumber					"Número de jugada"
::setup::board::mc::EnPassantFile				"Al paso"
::setup::board::mc::StartPosition				"Posición inicial"
::setup::board::mc::Fen								"FEN"
::setup::board::mc::Clear							"Vaciar"
::setup::board::mc::CopyFen						"Copiar FEN al portapapeles"
::setup::board::mc::Shuffle						"Shuffle..."
::setup::board::mc::StandardPosition			"Posición estándar"
::setup::board::mc::Chess960Castling			"Enroque en Chess 960"

::setup::board::mc::InvalidFen					"FEN no válido"
::setup::board::mc::CastlingWithoutRook		"Se han fijado derechos de enroque pero al menos una de las torres no está. Esto sólo sucede en partidas con ventaja. ¿Está seguro que los derechos de enroque son correctos?"
::setup::board::mc::UnsupportedVariant			"La posición es una posición de inicio pero no corresponde a una posición de Shuffle Chess. ¿Está seguro?"

::setup::board::mc::Error(InvalidFen)					"FEN no válido."
::setup::board::mc::Error(NoWhiteKing)					"Sin rey blanco."
::setup::board::mc::Error(NoBlackKing)					"Sin rey negro."
::setup::board::mc::Error(DoubleCheck)					"Ambos reyes en jaque."
::setup::board::mc::Error(OppositeCheck)				"El lado que no mueve está en jaque."
::setup::board::mc::Error(TooManyWhitePawns)			"Demasiados peones blancos."
::setup::board::mc::Error(TooManyBlackPawns)			"Demasiados peones negros."
::setup::board::mc::Error(TooManyWhitePieces)		"Demasiadas piezas blancas."
::setup::board::mc::Error(TooManyBlackPieces)		"Demasiadas piezas negras."
::setup::board::mc::Error(PawnsOn18)					"Peón en la 1ra o en la 8va fila."
::setup::board::mc::Error(TooManyKings)				"Más de dos reyes."
::setup::board::mc::Error(TooManyWhite)				"Demasiadas piezas blancas."
::setup::board::mc::Error(TooManyBlack)				"Demasiadas piezas negras."
::setup::board::mc::Error(BadCastlingRights)			"Derechos de enroque equivocados."
::setup::board::mc::Error(InvalidCastlingRights)	"Fila(s) de la torre irrazonable para el enroque."
::setup::board::mc::Error(InvalidCastlingFile)		"Fila no válida para el enroque."
::setup::board::mc::Error(AmbiguousCastlingFyles)	"El enroque requiere filas con torre para no ser ambiguo (posiblemente estén mal configuradas)."
::setup::board::mc::Error(InvalidEnPassant)			"Fila al paso no razonable."
::setup::board::mc::Error(MultiPawnCheck)				"Dos o más peones dando jaque."
::setup::board::mc::Error(TripleCheck)					"Tres o más piezas dando jaque."

### import #############################################################
::import::mc::ImportingPgnFile					"Importar archivo PGN '%s'"
::import::mc::Line									"Línea"
::import::mc::Column									"Columna"
::import::mc::GameNumber							"Partida"
::import::mc::ImportedGames						"%s partida(s) importada(s)"
::import::mc::NoGamesImported						"Ninguna partida importada"
::import::mc::FileIsEmpty							"el archivo probablemente está vacío"
::import::mc::PgnImport								"Importar PGN"
::import::mc::ImportPgnGame						"Importar partida PGN"
::import::mc::ImportPgnVariation					"Importar variante PGN"
::import::mc::ImportOK								"Texto PGN importado sin errores o advertencias."
::import::mc::ImportAborted						"Importación abortada."
::import::mc::TextIsEmpty							"El texto PGN está vacío."
::import::mc::AbortImport							"¿Abortar importación de PGN?"

::import::mc::DifferentEncoding					"La codificación seleccinada %src no coincide con la codificación del archivo %dst."
::import::mc::DifferentEncodingDetails			"Recodificar la base de datos no funcionará más despué de esta acción."
::import::mc::CannotDetectFigurineSet			"No puede auto-detectarse un juego de piezas adecuado."
::import::mc::CheckImportResult					"Por favor revise si se ha detectado el juego de piezas correcto."
::import::mc::CheckImportResultDetail			"En algunos casos la auto-detección falla debido a ambiguedades."

::import::mc::EnterOrPaste							"Ingrese o pegue en formato PGN %s en el cuadro de arriba.\nCualquier falla al importar el %s se mostrará aquí."
::import::mc::EnterOrPaste-Game					"partida"
::import::mc::EnterOrPaste-Variation			"variante"

::import::mc::MissingWhitePlayerTag				"Jugador de las Blancas desconocido"
::import::mc::MissingBlackPlayerTag				"Jugador de las Negras desconocido"
::import::mc::MissingPlayerTags					"Jugadores desconocidos"
::import::mc::MissingResult						"Resultado desconocido (al final de la sección de jugadas)"
::import::mc::MissingResultTag					"Resultado desconocido (en la sección encabezado)"
::import::mc::InvalidRoundTag						"Ronda no válida en el encabezado"
::import::mc::InvalidResultTag					"Resultado no válido en el encabezado"
::import::mc::InvalidDateTag						"Fecha no válida en el encabezado"
::import::mc::InvalidEventDateTag				"Fecha del evento no válida en el encabezado"
::import::mc::InvalidTimeModeTag					"Parámetros de tiempo no válidos en el encabezado"
::import::mc::InvalidEcoTag						"ECO no válido en el encabezado"
::import::mc::InvalidTagName						"Nombre no válido en el encabezado (ignorado)"
::import::mc::InvalidCountryCode					"Código de país no válido"
::import::mc::InvalidRating						"Número de rating no válido"
::import::mc::InvalidNag							"NAG no válido"
::import::mc::BraceSeenOutsideComment			"\"\}\" fuera de un comentario en la partida (se ignorarán)" 
::import::mc::MissingFen							"No start position for this Shuffle/Chess-960 game; will be interpreted as standard chess" ;# NEW
::import::mc::UnknownEventType					"Tipo de evento desconocido"
::import::mc::UnknownTitle							"Título desconocido (ignored)"
::import::mc::UnknownPlayerType					"Tipo de jugador desconocido (ignorado)"
::import::mc::UnknownSex							"Sexo desconocido (ignorado)"
::import::mc::UnknownTermination					"Motivo de la terminación desconocido"
::import::mc::UnknownMode							"Modo desconocido"
::import::mc::RatingTooHigh						"Número de valuación demasiado alto (ignorado)"
::import::mc::TooManyNags							"Demasiados NAG's (se ignorará el último)"
::import::mc::IllegalCastling						"Enroque ilegal"
::import::mc::IllegalMove							"Jugada ilegal"
::import::mc::CastlingCorrection					"Corrección del enroque"
::import::mc::UnsupportedVariant					"Variante de ajedrez no soportada"
::import::mc::DecodingFailed						"No se pudo decodificar esta partida"
::import::mc::ResultDidNotMatchHeaderResult	"El resultado no es igual al resultado del encabezado"
::import::mc::ValueTooLong							"El encabezado es demasiado largo y se cortará a los 255 caracteres"
::import::mc::MaximalErrorCountExceeded		"Máximo de errores excedido; no se informarán más errores (del tipo de error previo)"
::import::mc::MaximalWarningCountExceeded		"Máximo de advertencias excedido; no se informarán más advertencias (del tipo previo de advertencias)"
::import::mc::InvalidToken							"Símbolo no válido"
::import::mc::InvalidMove							"Jugada no válida"
::import::mc::UnexpectedSymbol					"Símbolo inesperado"
::import::mc::UnexpectedEndOfInput				"Inesperado final de la entrada de datos"
::import::mc::UnexpectedResultToken				"Símbolo de resultado inesperado"
::import::mc::UnexpectedTag						"Etiqueta inesperada dentro de la partida"
::import::mc::UnexpectedEndOfGame				"Final de la partida inesperado (resultado desconocido)"
::import::mc::TagNameExpected						"Error de sintaxis: se esperaba un nombre en el encabezado"
::import::mc::TagValueExpected					"Error de sintaxis: Se esperaba un valor en el encabezado"
::import::mc::InvalidFen							"FEN no válido"
::import::mc::UnterminatedString					"Flujo no determinado"
::import::mc::UnterminatedVariation				"Variante no determinada"
::import::mc::TooManyGames							"Demasiadas partidas en la base (abortado)"
::import::mc::GameTooLong							"Partida demasiado larga (omitida)"
::import::mc::FileSizeExceeded					"Se excede el tamaño máximo de archivo (2GB) (abortado)"
::import::mc::TooManyPlayerNames					"Demasiados nombres de jugadores en la base (abortado)"
::import::mc::TooManyEventNames					"Demasiados nombres de evento en la base (abortado)"
::import::mc::TooManySiteNames					"Demasiados nombres de lugares en la base (abortado)"
::import::mc::TooManyRoundNames					"Demasiados nombres de ronda en la base"
::import::mc::TooManyAnnotatorNames				"Demasiados nombres de comentaristas en la base (abortado)"
::import::mc::TooManySourceNames					"Demasiados nombres de orígenes en la base (abortado)"
::import::mc::SeemsNotToBePgnText				"No parece ser un texto PGN"
::import::mc::AbortedDueToInternalError		"Abortado debido a un error interno"
::import::mc::AbortedDueToIoError				"Aborted due to an read/write error" ;# NEW
::import::mc::UserHasInterrupted					"User has interrupted" ;# NEW

### export #############################################################
::export::mc::FileSelection				"&Selección de archivo"
::export::mc::OptionsSetup					"&Opciones"
::export::mc::PageSetup						"&Configuración de página"
::export::mc::DiagramSetup					"Configuración de &Diagrama"
::export::mc::StyleSetup					"&Estilo"
::export::mc::EncodingSetup				"Cod&ificación"
::export::mc::TagsSetup						"E&tiquetas"
::export::mc::NotationSetup				"&Notación"
::export::mc::AnnotationSetup				"Not&as"
::export::mc::CommentsSetup				"Co&mentarios"

::export::mc::Visibility					"Visibilidad"
::export::mc::HideDiagrams					"Ocultar Diagramas"
::export::mc::AllFromWhitePersp			"Desde la perspectiva de las Blancas"
::export::mc::AllFromBlackPersp			"Desde la perspectiva de las Negras"
::export::mc::ShowCoordinates				"Mostrar coordenadas"
::export::mc::ShowSideToMove				"Mostrar el lado que mueve"
::export::mc::ShowArrows					"Mostrar Flechas"
::export::mc::ShowMarkers					"Mostrar Marcedores"
::export::mc::Layout							"Disposición"
::export::mc::PostscriptSpecials			"Especialidades Postscript"
::export::mc::BoardSize						"Tamaño del tablero"

::export::mc::Short							"Corto"
::export::mc::Long							"Largo"
::export::mc::Algebraic						"Algebraico"
::export::mc::Correspondence				"Correspondencia"
::export::mc::Telegraphic					"Telegráfico"
::export::mc::FontHandling					"Manejo de fuentes"
::export::mc::DiagramStyle					"Estilo del Diagrama"
::export::mc::UseImagesForDiagram		"Usar imágenes para generar el diagrama"
::export::mc::EmebedTruetypeFonts		"Empotrar fuentes TrueType"
::export::mc::UseBuiltinFonts				"Usar fuentes incluidas"
::export::mc::SelectExportedTags			"Selección de etiquetas exportadas"
::export::mc::ExcludeAllTags				"Excluir todas las etiquetas"
::export::mc::IncludeAllTags				"Incluir todas las etiquetas"
::export::mc::ExtraTags						"Todas las etiquetas extra"
::export::mc::NoComments					"Sin comentarios"
::export::mc::AllLanguages					"Todos los idiomas"
::export::mc::Significant					"Significante"
::export::mc::LanguageSelection			"Selección de idioma"
::export::mc::MapTo							"Mapear a"
::export::mc::MapNagsToComment			"Mapear anotaciones a los comentarios"
::export::mc::UnusualAnnotation			"Anotaciones Inusuales"
::export::mc::AllAnnotation				"Todas las  anotaciones"
::export::mc::UseColumnStyle				"Usar estilo de columas"
::export::mc::MainlineStyle				"Estilo de Lúnea principal"
::export::mc::HideVariations				"Ocultar variaciones"

::export::mc::PdfFiles						"Archivos PDF"
::export::mc::HtmlFiles						"Archivos HTML"
::export::mc::TeXFiles						"Archivos LaTeX"

::export::mc::ExportDatabase				"Exportar base"
::export::mc::ExportDatabaseTitle		"Exportar base '%s'"
::export::mc::ExportingDatabase			"Exportando '%s' al archivo '%s'"
::export::mc::Export							"Exportar"
::export::mc::ExportedGames				"%s partida(s) exportada(s)"
::export::mc::NoGamesForExport			"No hay partidas para exportar."
::export::mc::ResetDefaults				"Volver a los parámetros predeterminados"
::export::mc::UnsupportedEncoding		"No use la codificación %s para documentos PDF. Debe elegir una codificación alternativa."
::export::mc::DatabaseIsOpen				"La base '%s' está abierta. Debe cerrarla primero."

::export::mc::BasicStyle					"Estilo básico"
::export::mc::GameInfo						"Información de la partida"
::export::mc::GameText						"Texto de la partida"
::export::mc::Moves							"Jugadas"
::export::mc::MainLine						"Línea principal"
::export::mc::Variation						"Variante"
::export::mc::Subvariation					"Subvariante"
::export::mc::Figurines						"Piezas"
::export::mc::Hyphenation					"Uso de guiones"
::export::mc::None							"(ninguno)"
::export::mc::Symbols						"Simbolos"
::export::mc::Comments						"Comentarios"
::export::mc::Result							"Resultado"
::export::mc::Diagram						"Diagrama"
::export::mc::ColumnStyle					"Estilo de Columnas"

::export::mc::Paper							"Papel"
::export::mc::Orientation					"Orientación"
::export::mc::Margin							"Márgenes"
::export::mc::Format							"Formato"
::export::mc::Size							"Tamaño"
::export::mc::Custom							"Habitual"
::export::mc::Potrait						"Retrato"
::export::mc::Landscape						"Apaisado"
::export::mc::Justification				"Justificado"
::export::mc::Even							"Ajustado"
::export::mc::Columns						"Columnas"
::export::mc::One								"Una"
::export::mc::Two								"Dos"

::export::mc::DocumentStyle				"Estilo del Documento"
::export::mc::Article						"Articulo"
::export::mc::Report							"Reporte"
::export::mc::Book							"Libro"

::export::mc::FormatName(scidb)			"Scidb"
::export::mc::FormatName(scid)			"Scid"
::export::mc::FormatName(pgn)				"PGN"
::export::mc::FormatName(pdf)				"PDF"
::export::mc::FormatName(html)			"HTML"
::export::mc::FormatName(tex)				"LaTeX"
::export::mc::FormatName(ps)				"Postscript"

::export::mc::Option(pgn,include_varations)						"Exportar variantes"
::export::mc::Option(pgn,include_comments)						"Exportar comentarios"
::export::mc::Option(pgn,include_moveinfo)						"Exportar información de las jugadas (como comentarios)"
::export::mc::Option(pgn,include_marks)							"Exportar marcadores (como comentarios)"
::export::mc::Option(pgn,use_scidb_import_format)				"Usar el formato de importación de Scidb"
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
::export::mc::Option(pgn,column_style)								"Estilo columna (una jugada por línea)"
::export::mc::Option(pgn,symbolic_annotation_style)			"Estilo simbólico de comentarios (!, !?)"
::export::mc::Option(pgn,extended_symbolic_style)				"Estilo simbólico extendido de comentarios (+=, +/-)"
::export::mc::Option(pgn,convert_null_moves)						"Convertir las jugadas nulas en comentarios"
::export::mc::Option(pgn,space_after_move_number)				"Agregar espacio tras los números de jugada"
::export::mc::Option(pgn,shredder_fen)								"Escribir Shredder-FEN (X-FEN es lo predeterminado)"
::export::mc::Option(pgn,convert_lost_result_to_comment)		"Escribir comentario para el resultado '0-0'"
::export::mc::Option(pgn,append_mode_to_event_type)			"Agregar modo tras el tipo de evento"
::export::mc::Option(pgn,comment_to_html)							"Escribir comentario en estilo HTML"
::export::mc::Option(pgn,exclude_games_with_illegal_moves)	"Excluir partidas con jugadas ilegales"
::export::mc::Option(pgn,use_utf8_encoding)						"Use UTF-8 encoding" ;# NEW

### notation ###########################################################
::notation::mc::Notation		"Notación"

::notation::mc::MoveForm(alg)	"Algebraica" 
::notation::mc::MoveForm(san)	"Algebraica Corta" 
::notation::mc::MoveForm(lan)	"Algebraica Larga" 
::notation::mc::MoveForm(eng)	"Inglés" 
::notation::mc::MoveForm(cor)	"Correspondencia" 
::notation::mc::MoveForm(tel)	"Telegrafico" 

### figurine ###########################################################
::figurines::mc::Figurines	"Piezas"
::figurines::mc::Graphic	"Gráficos"
::figurines::mc::User		"User" ;# NEW meaning is "user defined"

### save/replace #######################################################
::dialog::save::mc::SaveGame						"Guardar partida"
::dialog::save::mc::ReplaceGame					"Reemplazar partida"
::dialog::save::mc::EditCharacteristics		"Editar características"
	
::dialog::save::mc::GameData						"Datos de la partida"
::dialog::save::mc::Event							"Evento"

::dialog::save::mc::MatchesExtraTags			"Etiquetas iguales / Extra Etiquetas"
::dialog::save::mc::PressToSelect				"Presione Ctrl+0 a Ctrl+9 (o el botón izquierdo del ratón) para seleccionar"
::dialog::save::mc::PressForWhole				"Presione Alt-0 a Alt-9 (o el botón medio del ratón) para grupo de datos completo"
::dialog::save::mc::EditTags						"Editar etiquetas"
::dialog::save::mc::RemoveThisTag				"Eliminar etiqueta '%s'?"
::dialog::save::mc::TagAlreadyExists			"El nombre de etiqueta '%s' ya existe."
::dialog::save::mc::TagRemoved					"La etiqueta sobrante '%s' (valor actual: '%s') será eliminada."
::dialog::save::mc::TagNameIsReserved			"El nombre de etiqueta '%s' está reservado."
::dialog::save::mc::Locked							"Bloqueado"
::dialog::save::mc::OtherTag						"Otra etiqueta"
::dialog::save::mc::NewTag							"Agregar nueva etiqueta"
::dialog::save::mc::RemoveTag						"Etiqueta eliminada"
::dialog::save::mc::SetToGameDate				"Establecer fecha de la partida"
::dialog::save::mc::SaveGameFailed				"Guardado de la partida fallido."
::dialog::save::mc::SaveGameFailedDetail		"Ver bitácora para los detalles."
::dialog::save::mc::SavingGameLogInfo			"Guardar partida (%white - %black, %event) en base '%base'"
::dialog::save::mc::CurrentBaseIsReadonly		"La base actual '%s' es de sólo lectura."
::dialog::save::mc::CurrentGameHasTrialMode	"El juego actual está en modo de prueba y no puede ser guardado."

::dialog::save::mc::LocalName						"&Nombre local"
::dialog::save::mc::EnglishName					"Nombre I&nglés"
::dialog::save::mc::ShowRatingType				"Mostrar &rating"
::dialog::save::mc::EcoCode						"Código &ECO"
::dialog::save::mc::Matches						"&Matches"
::dialog::save::mc::Tags							"E&tiquetas"

::dialog::save::mc::Label(name)					"Nombre"
::dialog::save::mc::Label(fideID)				"Fide-ID"
::dialog::save::mc::Label(value)					"Valor"
::dialog::save::mc::Label(title)					"Título"
::dialog::save::mc::Label(rating)				"Rating"
::dialog::save::mc::Label(federation)			"Federación"
::dialog::save::mc::Label(country)				"País"
::dialog::save::mc::Label(eventType)			"Tipo"
::dialog::save::mc::Label(sex)					"Sexo/Tipo"
::dialog::save::mc::Label(date)					"Fecha"
::dialog::save::mc::Label(eventDate)			"Fecha del evento"
::dialog::save::mc::Label(round)					"Ronda"
::dialog::save::mc::Label(result)				"Resultado"
::dialog::save::mc::Label(termination)			"Terminación"
::dialog::save::mc::Label(annotator)			"Comentarista"
::dialog::save::mc::Label(site)					"Lugar"
::dialog::save::mc::Label(eventMode)			"Modo"
::dialog::save::mc::Label(timeMode)				"Tiempo"
::dialog::save::mc::Label(frequency)			"Frecuencia"
::dialog::save::mc::Label(score)					"Rating secundario"

::dialog::save::mc::GameBase						"Base de partidas"
::dialog::save::mc::PlayerBase					"Base de jugadores"
::dialog::save::mc::EventBase						"Base de eventos"
::dialog::save::mc::SiteBase						"Base de lugares"
::dialog::save::mc::AnnotatorBase				"Base de comentaristas"
::dialog::save::mc::History						"Historial"

::dialog::save::mc::InvalidEntry					"'%s' no es una entrada válida."
::dialog::save::mc::InvalidRoundEntry			"'%s' no es una entrada de ronda válida."
::dialog::save::mc::InvalidRoundEntryDetail	"Entradas de ronda válidas son '4' ó '6.1'. No se permiten ceros."
::dialog::save::mc::RoundIsTooHigh				"La ronda debería ser menor a 256."
::dialog::save::mc::SubroundIsTooHigh			"La sub-ronda debería ser menor a 256."
::dialog::save::mc::ImplausibleDate				"La fecha de la partida '%s' es anterior a la fecha del evento '%s'."
::dialog::save::mc::InvalidTagName				"Nombre de etiqueta no válido '%s' (error de sintaxis)."
::dialog::save::mc::Field							"Campo '%s': "
::dialog::save::mc::ExtraTag						"Etiqueta extra '%s': "
::dialog::save::mc::InvalidNetworkAddress		"Dirección de red '%s' no válida."
::dialog::save::mc::InvalidCountryCode			"Código de país '%s' no válido."
::dialog::save::mc::InvalidEventRounds			"Número de rondas del evento '%s' no válido (se espera un número entero positivo)."
::dialog::save::mc::InvalidPlyCount				"Recuento de jugadas '%s' no válido (se espera un número entero positivo)."
::dialog::save::mc::IncorrectPlyCount			"Recuento de jugadas '%s' incorrecto (el recuento de jugadas real es %s)."
::dialog::save::mc::InvalidTimeControl			"Entrada del campo de control de tiempo en '%s' no válida."
::dialog::save::mc::InvalidDate					"Fecha '%s' no válida."
::dialog::save::mc::InvalidYear					"Año '%s' no válido."
::dialog::save::mc::InvalidMonth					"Mes '%s' no válido."
::dialog::save::mc::InvalidDay					"Día '%s' no válido."
::dialog::save::mc::MissingYear					"Se desconoce el año."
::dialog::save::mc::MissingMonth					"Se desconoce el mes."
::dialog::save::mc::StringTooLong				"Etiqueta %tag%: la cadena '%value%' es demasiado larga y se cortará a '%trunc%'."
::dialog::save::mc::InvalidEventDate			"No se puede aceptar la fecha de evento suministrada: La diferencia entre el año de la partida y el año del evento debería ser menor a 4 (restricción del formato de base de datos de Scid)."
::dialog::save::mc::TagIsEmpty					"La etiqueta '%s' está vacía (se descartará)."

### gamehistory ########################################################
::game::history::mc::GameHistory	"Historial de Juegos"

### game ###############################################################
::game::mc::CloseDatabase					"Cerrar Base"
::game::mc::CloseAllGames					"¿Cerrar todas las partidas abiertas de la base '%s'?"
::game::mc::SomeGamesAreModified			"Se modificaron algunas partidas de la base '%s'. ¿Cerrar de todos modos?" 
::game::mc::AllSlotsOccupied				"Todos los espacios para partidas están ocupados."
::game::mc::ReleaseOneGame					"Por favor, saque una de las partidas antes de cargar otra."
::game::mc::GameAlreadyOpen				"La partida ya está abierta pero fue modificada. ¿Descartar la versión modificada de esta partida?"
::game::mc::GameAlreadyOpenDetail		"'%s' abrirá una nueva partida."
::game::mc::GameHasChanged					"La partida %s se ha modificado."
::game::mc::GameHasChangedDetail			"Probablemente este no es el juego esperado debido al cambio de bases de datos."
::game::mc::CorruptedHeader				"Encabezado corrupto en el archivo de recuperación '%s'."
::game::mc::RenamedFile						"Renombrar este archivo como '%s.bak'."
::game::mc::CannotOpen						"No se puede abrir el archivo de recuperación '%s'."
::game::mc::GameRestored					"Una partida restablecida de la última sesión."
::game::mc::GamesRestored					"%s partidas restablecidas de la última sesión."
::game::mc::OldGameRestored				"Una partida restablecida."
::game::mc::OldGamesRestored				"%s partidas restablecidas."
::game::mc::ErrorInRecoveryFile			"Error en el archivo de recuperación '%s'"
::game::mc::Recovery							"Recuperación"
::game::mc::UnsavedGames					"Usted tiene cambios en la partida que no se han guardado."
::game::mc::DiscardChanges					"'%s' descartará todos los cambios."
::game::mc::ShouldRestoreGame				"¿Debería restablecerse esta partida en la próxima sesión?"
::game::mc::ShouldRestoreGames			"¿Deberían restablecerse estas partidas en la sesión siguiente?"
::game::mc::NewGame							"Nueva partida"
::game::mc::NewGames							"Nuevas partidas"
::game::mc::Created							"creado"
::game::mc::ClearHistory					"Limpiar Historial"
::game::mc::RemoveSelectedGame			"Remover el juego seleccionado del historial"
::game::mc::GameDataCorrupted				"Los datos de la partida son erróneos."
::game::mc::GameDecodingFailed			"No se pudo decodificar esta partida."
::game::mc::GameDecodingChanged			"La base de datos fue abierta usando el juego de caracteres '%base%', pero esta partida parece estar codificada usando '%game%', por ello, esta partida se cargará usando el juego de caracteres detectado."
::game::mc::GameDecodingChangedDetail	"Probablemente se abriá la base de datos con el juego de caracteres equivocado. Note que la detección automática del juego de caracteres es limitada."

### languagebox ########################################################
::languagebox::mc::AllLanguages	"Todos los idiomas"
::languagebox::mc::None				"Ninguno"

### datebox ############################################################
::widget::datebox::mc::Today		"Hoy"
::widget::datebox::mc::Calendar	"Calendario..."
::widget::datebox::mc::Year		"Año"
::widget::datebox::mc::Month		"Mes"
::widget::datebox::mc::Day			"Día"

### genderbox ##########################################################
::genderbox::mc::Gender(m) "Masculino"
::genderbox::mc::Gender(f) "Femenino"
::genderbox::mc::Gender(c) "Computadora"

### terminationbox #####################################################
::terminationbox::mc::Normal				"Normal"
::terminationbox::mc::Unplayed			"No jugado"
::terminationbox::mc::Abandoned			"Abandona"
::terminationbox::mc::Adjudication		"Adjudicación"
::terminationbox::mc::Death				"Muerte"
::terminationbox::mc::Emergency			"Emergencia"
::terminationbox::mc::RulesInfraction	"Infracción a las reglas"
::terminationbox::mc::TimeForfeit		"Pierde por tiempo"
::terminationbox::mc::Unterminated		"No terminada"

### eventmodebox #######################################################
::eventmodebox::mc::OTB				"En tablero"
::eventmodebox::mc::PM				"Correspondencia"
::eventmodebox::mc::EM				"E-mail"
::eventmodebox::mc::ICS				"Internet Chess Server"
::eventmodebox::mc::TC				"Telecomunicación"
::eventmodebox::mc::Analysis		"Análisis"
::eventmodebox::mc::Composition	"Composición"

### eventtypebox #######################################################
::eventtypebox::mc::Type(game)	"Partida individual"
::eventtypebox::mc::Type(match)	"Match"
::eventtypebox::mc::Type(tourn)	"Round Robin"
::eventtypebox::mc::Type(swiss)	"Torneo por sistema suizo"
::eventtypebox::mc::Type(team)	"Torneo por equipos"
::eventtypebox::mc::Type(k.o.)	"Torneo por Knockout"
::eventtypebox::mc::Type(simul)	"Torneo de simultáneas"
::eventtypebox::mc::Type(schev)	"Torneo por sistema Scheveningen"  

### timemodebox ########################################################
::timemodebox::mc::Mode(normal)	"Normal"
::timemodebox::mc::Mode(rapid)	"Rápidas"
::timemodebox::mc::Mode(blitz)	"Blitz"
::timemodebox::mc::Mode(bullet)	"Bullet"
::timemodebox::mc::Mode(corr)		"Correspondencia"

### help ###############################################################
::help::mc::Contents					"&Contenido"
::help::mc::Index						"&Indice"
::help::mc::Search					"Bu&scar"

::help::mc::Help						"Ayuda"
::help::mc::MatchEntireWord		"Coincidir palabra completa"
::help::mc::MatchCase				"Coincidir capitalización"
::help::mc::TitleOnly				"Buscar solamente en tútulos"
::help::mc::CurrentPageOnly		"Buscar solamente en la página actual"
::help::mc::GoBack					"Retroceder una página (Alt-\u2190)"
::help::mc::GoForward				"Avanzar una página (Alt-\u2192)"
::help::mc::GotoPage					"Ir a la página '%s'"
::help::mc::ExpandAllItems			"Expandir todo"
::help::mc::CollapseAllItems		"Colapsar todo"
::help::mc::SelectLanguage			"Seleccionar idioma"
::help::mc::NoHelpAvailable		"No hay archivos de ayuda en Español.\nPor favor seleccione un lenguaje alternativo\npara el diálogo de ayuda."
::help::mc::NoHelpAvailableAtAll	"No help files available for this topic."
::help::mc::KeepLanguage			"¿Mantener el idioma  %s para sesiones futuras?"
::help::mc::ParserError				"Error al analizar archivo %s."
::help::mc::NoMatch					"No se encontró una coincidencia"
::help::mc::MaxmimumExceeded		"Se excedió el número máximo de coincidencias en algunas páginas."
::help::mc::OnlyFirstMatches		"Solo se mostrarán las primeras %s coincidencias por página."
::help::mc::HideIndex				"Ocultar el úndice"
::help::mc::ShowIndex				"Mostrar el úndice"

::help::mc::FileNotFound			"No se encontró el archivo."
::help::mc::CantFindFile			"No se encuentrá el archivo en %s."
::help::mc::IncompleteHelpFiles	"Lo sentimos pero parece que los archivos de ayuda aún estan incompletos."
::help::mc::ProbablyTheHelp		"Probablemente la página de ayuda en un lenguaje diferente puede ser una alternativa para usted"
::help::mc::PageNotAvailable		"Esta página no se encuentra disponible"

::help::mc::Overview					"Resumen"

### crosstable #########################################################
::crosstable::mc::TournamentTable		"Tabla del torneo"
::crosstable::mc::AverageRating			"Rating promedio"
::crosstable::mc::Category					"Categoría"
::crosstable::mc::Games						"partidas"
::crosstable::mc::Game						"partida"

::crosstable::mc::ScoringSystem			"Sistema de Punteo"
::crosstable::mc::Tiebreak					"Desempate"
::crosstable::mc::Settings					"Configuraciones"
::crosstable::mc::RevertToStart			"Volver a los valores iniciales"
::crosstable::mc::UpdateDisplay			"Actualizar el visor"

::crosstable::mc::Traditional				"Tradicional"
::crosstable::mc::Bilbao					"Bilbao"

::crosstable::mc::None						"Ninguno"
::crosstable::mc::Buchholz					"Buchholz"
::crosstable::mc::MedianBuchholz			"Buchholz-Mediano"
::crosstable::mc::ModifiedMedianBuchholz "Buchholz-Mediano Mod."
::crosstable::mc::RefinedBuchholz		"Buchholz perfeccionado"
::crosstable::mc::SonnebornBerger		"Sonneborn-Berger"
::crosstable::mc::Progressive				"Puntajes progresivos"
::crosstable::mc::KoyaSystem				"Sistema Koya"
::crosstable::mc::GamesWon					"Número de partidas ganadas"
::crosstable::mc::GamesWonWithBlack		"Partidas Ganadas con Negras"
::crosstable::mc::ParticularResult		"Resultado Particular"
::crosstable::mc::TraditionalScoring	"Punteo Tradicional"

::crosstable::mc::Crosstable				"Cuadro cruzado"
::crosstable::mc::Scheveningen			"Scheveningen"
::crosstable::mc::Swiss						"Sistema suizo"
::crosstable::mc::Match						"Match"
::crosstable::mc::Knockout					"Knockout"
::crosstable::mc::RankingList				"Lista de Ranking"

::crosstable::mc::Order						"Orden"
::crosstable::mc::Type						"Tipo tabla"
::crosstable::mc::Score						"Puntuación"
::crosstable::mc::Alphabetical			"Alfabético"
::crosstable::mc::Rating					"Rating"
::crosstable::mc::Federation				"Federación"

::crosstable::mc::Debugging				"Depuración"
::crosstable::mc::Display					"Visor"
::crosstable::mc::Style						"Estilo"
::crosstable::mc::Spacing					"Espaciado"
::crosstable::mc::Padding					"Relleno"
::crosstable::mc::ShowLog					"Mostrar bitácora"
::crosstable::mc::ShowHtml					"Mostrar HTML"
::crosstable::mc::ShowRating				"Rating"
::crosstable::mc::ShowPerformance		"Desempeño"
::crosstable::mc::ShowWinDrawLoss		"Ganadas/Tablas/Perdidas"
::crosstable::mc::ShowTiebreak			"Desempate"
::crosstable::mc::ShowOpponent			"Oponente (como Tooltip)"
::crosstable::mc::KnockoutStyle			"Estilo tabla de Knockout"
::crosstable::mc::Pyramid					"Pirámide"
::crosstable::mc::Triangle					"Triángulo"

::crosstable::mc::CrosstableLimit		"Se excederá el límite del cuadro cruzado de %d jugadores."
::crosstable::mc::CrosstableLimitDetail "'%s' está seleccionando otro modo de tabla."

### info ###############################################################
::info::mc::InfoTitle			"Acerca de %s"
::info::mc::Info					"Información"
::info::mc::About					"Acerca de"
::info::mc::Contributions		"Contribuciones"
::info::mc::License				"Licencia"
::info::mc::Localization		"Localización"
::info::mc::Testing				"Pruebas"
::info::mc::References			"Referencias"
::info::mc::System				"Sistema"
::info::mc::FontDesign			"Diseño de fuentes"
::info::mc::ChessPieceDesign	"Diseño de las piezas"
::info::mc::BoardThemeDesign	"Diseño de tema de tablero"
::info::mc::FlagsDesign			"Diseño de las banderas en miniatura"
::info::mc::IconDesign			"Diseño de iconos"
::info::mc::Development			"Development" ;# NEW
::info::mc::Programming			"Programming" ;# NEW
::info::mc::Leader				"Leader" ;# NEW

::info::mc::Version				"Versión"
::info::mc::Distributed			"Este programa se distribuye bajo los términos de la Licencia Pública General GNU."
::info::mc::Inspired				"Scidb está inspirado en Scid 3.6.1, registrado en \u00A9 1999-2003 por Shane Hudson."
::info::mc::SpecialThanks		"Un especial agradecimiento a Shane Hudson por su estupendo trabajo. Su empeño constituye la base de esta aplicación."

### comment ############################################################
::comment::mc::CommentBeforeMove		"Comentario antes de la jugada"
::comment::mc::CommentAfterMove		"Comentario tras la jugada"
::comment::mc::PrecedingComment		"Comentario precedente"
::comment::mc::TrailingComment		"Último comentario"
::comment::mc::Language					"Idioma"
::comment::mc::AddLanguage				"Agregar idioma..."
::comment::mc::SwitchLanguage			"Cambiar idioma"
::comment::mc::FormatText				"Dar formato al texto"
::comment::mc::CopyText					"Copiar texto a"
::comment::mc::OverwriteContent		"¿Sobrescribir contenido existente?"
::comment::mc::AppendContent			"Si \"no\" el texto se agregará al final."

::comment::mc::LanguageSelection		"Selección de idioma"
::comment::mc::Formatting				"Formateando"

::comment::mc::Bold						"Negrita"
::comment::mc::Italic					"Itálica"
::comment::mc::Underline				"Subrayado"

::comment::mc::InsertSymbol			"&Insertar símbolo..."
::comment::mc::MiscellaneousSymbols	"Simbolos misceláneos"
::comment::mc::Figurine					"Piezas"

### annotation #########################################################
::annotation::mc::AnnotationEditor					"Notas"
::annotation::mc::TooManyNags							"Demasiados comentarios (el último será ignorado)."
::annotation::mc::TooManyNagsDetail					"Se permite un máximo de %d comentarios por jugada individual."

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
::move::mc::NewMainLine				"Nueva línea principal"
::move::mc::TryVariation			"Probar variante"
::move::mc::ExchangeMove			"Cambiar jugada"

::move::mc::GameWillBeTruncated	"Se truncará la partida. ¿Continuar con '%s'?"

### log ################################################################
::log::mc::LogTitle		"Bitácora"
::log::mc::Warning		"Advertencia"
::log::mc::Error			"Error"
::log::mc::Information	"Información"

### titlebox ############################################################
::titlebox::mc::Title(GM)		"Gran Maestro (FIDE)"
::titlebox::mc::Title(IM)		"Maestro Internacional (FIDE)"
::titlebox::mc::Title(FM)		"Maestro Fide (FIDE)"
::titlebox::mc::Title(CM)		"Candidato a Maestro (FIDE)"
::titlebox::mc::Title(WGM)		"Gran Maestro Femenino (FIDE)"
::titlebox::mc::Title(WIM)		"Maestro Internacional Femenino (FIDE)"
::titlebox::mc::Title(WFM)		"Maestro Fide Femenino (FIDE)"
::titlebox::mc::Title(WCM)		"Candidato a Maestro Femenino (FIDE)"
::titlebox::mc::Title(HGM)		"Gran Maestro Honorario (FIDE)"
::titlebox::mc::Title(NM)		"Maestro Nacional (USCF)"
::titlebox::mc::Title(SM)		"Maestro Senior (USCF)"
::titlebox::mc::Title(LM)		"Maestro de por Vida (USCF)"
::titlebox::mc::Title(CGM)		"Gran Maestro por Correspondencia (ICCF)"
::titlebox::mc::Title(CIM)		"Maestro Internacional por Correspondencia (ICCF)"
::titlebox::mc::Title(CLGM)	"Gran Maestro Femenino por Correspondencia (ICCF)"
::titlebox::mc::Title(CILM)	"Maestro Internacional Femenino por Correspondencia (ICCF)"
::titlebox::mc::Title(CSIM)	"Maestro Internacional Senior por Correspondencia (ICCF)"

### messagebox #########################################################
::dialog::mc::Ok				"&Aceptar"
::dialog::mc::Cancel			"&Cancelar"
::dialog::mc::Yes				"&Sí"
::dialog::mc::No				"&No"
::dialog::mc::Retry			"&Reintentar"
::dialog::mc::Abort			"A&bortar"
::dialog::mc::Ignore			"&Ignorar"
::dialog::mc::Continue		"Con&tinuar"

::dialog::mc::Error			"Error"
::dialog::mc::Warning		"Advertencia"
::dialog::mc::Information	"Información"
::dialog::mc::Question		"Confirmar"

::dialog::mc::DontAskAgain	"No pregunte nuevamente"

### web ################################################################
::web::mc::CannotFindBrowser			"No se pudo encontrar un navegador."
::web::mc::CannotFindBrowserDetail	"Configure la variable de entorno BROWSER a su navegador deseado."

### colormenu ##########################################################
::colormenu::mc::BaseColor			"Color base"
::colormenu::mc::UserColor			"Color del usuario"
::colormenu::mc::UsedColor			"Color utilizado"
::colormenu::mc::RecentColor		"Color reciente"
::colormenu::mc::Texture			"Textura"
::colormenu::mc::OpenColorDialog	"Abrir el diálogo de colores"
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
::table::mc::MinWidth					"Ancho mínimo"
::table::mc::MaxWidth					"Ancho máximo"
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

### fileselectionbox ###################################################
::dialog::fsbox::mc::ScidbDatabase			"Base de datos Scidb"
::dialog::fsbox::mc::ScidDatabase			"Base de datos Scid"
::dialog::fsbox::mc::ChessBaseDatabase		"Base de datos ChessBase" 
::dialog::fsbox::mc::PortableGameFile		"Archivo PGN"
::dialog::fsbox::mc::ZipArchive				"Archivo ZIP" 
::dialog::fsbox::mc::ScidbArchive			"Archivo Scidb" 
::dialog::fsbox::mc::PortableDocumentFile	"Archivo PDF" 
::dialog::fsbox::mc::HypertextFile			"Archivo HTML"
::dialog::fsbox::mc::TypesettingFile		"Archivo LATEX"
::dialog::fsbox::mc::ImageFile				"Image File" ;# NEW
::dialog::fsbox::mc::LinkTo					"Vúnculo a %s" 
::dialog::fsbox::mc::LinkTarget				"Destino del vúnculo" 
::dialog::fsbox::mc::Directory				"Directorio" 

::dialog::fsbox::mc::Content					"Contenido" 
::dialog::fsbox::mc::Open						"Abrir" 

### choosecolor ########################################################
::dialog::choosecolor::mc::Ok					"&Aceptar"
::dialog::choosecolor::mc::Cancel			"&Cancelar"

::dialog::choosecolor::mc::BaseColors		"Colores base"
::dialog::choosecolor::mc::UserColors		"Colores del usuario"
::dialog::choosecolor::mc::RecentColors	"Colores recientes"
::dialog::choosecolor::mc::Old				"Antiguo"
::dialog::choosecolor::mc::Current			"Actual"
::dialog::choosecolor::mc::HexCode			"Hex Code" ;# NEW
::dialog::choosecolor::mc::ColorSelection	"Elección del color"
::dialog::choosecolor::mc::Red				"Rojo"
::dialog::choosecolor::mc::Green				"Verde"
::dialog::choosecolor::mc::Blue				"Azul"
::dialog::choosecolor::mc::Hue				"Tono"
::dialog::choosecolor::mc::Saturation		"Saturación"
::dialog::choosecolor::mc::Value				"Valor"
::dialog::choosecolor::mc::Enter				"Ingresar"
::dialog::choosecolor::mc::AddColor			"Agregar el color actual a los colores de usuario"

### choosefont #########################################################
::dialog::choosefont::mc::Apply				"&Aplicar"
::dialog::choosefont::mc::Cancel				"&Cancelar"
::dialog::choosefont::mc::Continue			"Con&tinuar"
::dialog::choosefont::mc::FixedOnly			"Sólo fuentes m&onoespaciadas/fijas"
::dialog::choosefont::mc::Family				"Fam&ilia"
::dialog::choosefont::mc::Font				"&Fuente"
::dialog::choosefont::mc::Ok					"Ac&eptar"
::dialog::choosefont::mc::Reset				"&Reajustar"
::dialog::choosefont::mc::Size				"&Tamaño"
::dialog::choosefont::mc::Strikeout			"Tach&ado"
::dialog::choosefont::mc::Style				"Es&tilo"
::dialog::choosefont::mc::Underline			"S&ubrayado"
::dialog::choosefont::mc::Color				"Color"

::dialog::choosefont::mc::Regular			"Regular" 
::dialog::choosefont::mc::Bold				"Negrita" 
::dialog::choosefont::mc::Italic				"Italica" 
{::dialog::choosefont::mc::Bold Italic}	"Negrita Italica" 

::dialog::choosefont::mc::Effects			"Efectos"
::dialog::choosefont::mc::Filter				"Filtro"
::dialog::choosefont::mc::Sample				"Muestra"
::dialog::choosefont::mc::SearchTitle		"Buscando fuentes monoespaciadas/fijas"
::dialog::choosefont::mc::SeveralMinutes	"Esta operación puede tomar cerca de %d minuto(s)."
::dialog::choosefont::mc::FontSelection	"Selección de fuente"
::dialog::choosefont::mc::Wait				"Espere"

### choosedir ##########################################################
::choosedir::mc::ShowPredecessor	"Mostrar Predecesor"
::choosedir::mc::ShowTail			"Mostrar Atrasados"
::choosedir::mc::Folder				"Folder"

### fsbox ##############################################################
::fsbox::mc::Name								"Nombre"
::fsbox::mc::Size								"Tamaño"
::fsbox::mc::Modified						"Modificado"

::fsbox::mc::Forward							"Continuar a '%s'"
::fsbox::mc::Backward						"Retroceder a '%s'"
::fsbox::mc::Delete							"Eliminar"
::fsbox::mc::MoveToTrash					"Move to Trash" ;# NEW
::fsbox::mc::Restore							"Restaurar"
::fsbox::mc::Duplicate						"Duplicar"
::fsbox::mc::CopyOf							"Copia de %s"
::fsbox::mc::NewFolder						"Nueva Carpeta"
::fsbox::mc::Layout							"Disposición"
::fsbox::mc::ListLayout						"Formato de Lista"
::fsbox::mc::DetailedLayout				"Formato Detallado"
::fsbox::mc::ShowHiddenDirs				"&Mostrar directorios ocultos"
::fsbox::mc::ShowHiddenFiles				"&Mostrar archivos y directorios ocultos"
::fsbox::mc::AppendToExisitingFile		"&Agregar partidas a un archivo existente"
::fsbox::mc::Cancel							"&Cancelar"
::fsbox::mc::Save								"&Guardar"
::fsbox::mc::Open								"&Abrir"
::fsbox::mc::Overwrite						"&Overwrite" ;# NEW
::fsbox::mc::Rename							"&Renombrar"
::fsbox::mc::Move								"Move" ;# NEW

::fsbox::mc::AddBookmark					"Agregar Marcador '%s'"
::fsbox::mc::RemoveBookmark				"Quitar Marcador '%s'"
::fsbox::mc::RenameBookmark				"Renombrar Marcador '%s'"

::fsbox::mc::Filename						"&Nombre del archivo:"
::fsbox::mc::Filenames						"&Nombres del archivo:"
::fsbox::mc::Directory						"&Directorio:"
::fsbox::mc::FilesType						"&Tipo de archivos:"
::fsbox::mc::FileEncoding					"Codificación de archivo:"

::fsbox::mc::Favorites						"Favoritoes"
::fsbox::mc::LastVisited					"Visitado por última vez"
::fsbox::mc::FileSystem						"Sistema de archivos"
::fsbox::mc::Desktop							"Escritorio"
::fsbox::mc::Trash							"Papelera"
::fsbox::mc::Home								"Inicio"

::fsbox::mc::SelectEncoding				"Seleccionar la codificación de la base de datos (abre un cuadro de dialogo)"
::fsbox::mc::SelectWhichType				"Elegir qué tipo de archivo mostrar"
::fsbox::mc::TimeFormat						"%d/%m/%y %I:%M %p"

::fsbox::mc::CannotChangeDir				"No se puede cambiar al directorio '%s'.\nPermiso denegado."
::fsbox::mc::DirectoryRemoved				"No se puede cambiar al directorio '%s'.\nEl directorio fue eliminado."
::fsbox::mc::DeleteFailed					"No se pudo borrar '%s'."
::fsbox::mc::RestoreFailed					"No se pudo restaurar '%s'."
::fsbox::mc::CommandFailed					"Falló el comando '%s'."
::fsbox::mc::CopyFailed						"No se pudo copiar el archivo '%s': permiso denegado."
::fsbox::mc::CannotCopy						"No se puede crear una copia porque el archivo '%s' ya existe."
::fsbox::mc::CannotDuplicate				"No se puede duplicar el archivo '%s': no se tiene permisos de lectura."
::fsbox::mc::ReallyDuplicateFile			"¿Realmente desea duplicar este archivo?"
::fsbox::mc::ReallyDuplicateDetail		"Este archivo tiene aproximadamente %s. Duplicarlo puede tardar algún tiempo."
::fsbox::mc::InvalidFileExt				"Operación falló: '%s' tiene una extensión inválida."
::fsbox::mc::CannotRename					"No se puede cambiar el nombre a '%s' porque la carpeta/archivo ya existe."
::fsbox::mc::CannotCreate					"No se puede crear la carpeta '%s' porque ya existe."
::fsbox::mc::ErrorCreate					"Error creando la carpeta: permiso denegado."
::fsbox::mc::FilenameNotAllowed			"El nombre de archivo '%s' no se permite."
::fsbox::mc::ContainsTwoDots				"Contiene dos puntos consecutivos."
::fsbox::mc::ContainsReservedChars		"Contiene caracteres reservados: %s, o un caracter de control (ASCII 0-31)."
::fsbox::mc::InvalidFileName				"El nombre de archivo no puede comenzar con un guión, y no puede terminar con un espacio o punto."
::fsbox::mc::IsReservedName				"En algunos sistemas operativos este es un nombre reservado."
::fsbox::mc::FilenameTooLong				"Un nombre de archivo debe tener menos de 256 caracteres."
::fsbox::mc::InvalidFileExtension		"Extensión de archivo inválida en '%s'."
::fsbox::mc::MissingFileExtension		"El archivo '%s' no tiene extensión."
::fsbox::mc::FileAlreadyExists			"El archivo '%s' ya existe.\n¿Quiere sobreescribirlo?"
::fsbox::mc::CannotOverwriteDirectory	"No se puede sobrescribir el directorio '%s'."
::fsbox::mc::FileDoesNotExist				"El archivo '%s' no existe."
::fsbox::mc::DirectoryDoesNotExist		"El directorio '%s' no existe."
::fsbox::mc::CannotOpenOrCreate			"No se puede abrir/crear '%s'. Por favor escoja un directorio."
::fsbox::mc::WaitWhileDuplicating		"Por favor espere mientras se duplica el archivo..."
::fsbox::mc::FileHasDisappeared			"El archivo '%s' ha desaparecido."
::fsbox::mc::CurrentlyInUse				"El archivo se encuentra en uso."
::fsbox::mc::PermissionDenied				"Permiso denegado para el directorio '%s'."
::fsbox::mc::CannotOpenUri					"No se puede abrir la siguiente URI:"
::fsbox::mc::InvalidUri						"Descartar contenido no es una lista URI válida."
::fsbox::mc::UriRejected					"Los siguientes archivos fueron rechazados:"
::fsbox::mc::UriRejectedDetail			"Only the listed file types can be handled." ;# NEW
::fsbox::mc::OperationAborted				"Operation aborted." ;# NEW
::fsbox::mc::ApplyOnDirectories			"Are you sure that you want to apply the selected operation on (the following) directories?" ;# NEW
::fsbox::mc::EntryAlreadyExists			"Entry already exists" ;# NEW
::fsbox::mc::AnEntryAlreadyExists		"An entry '%s' already exists." ;# NEW
::fsbox::mc::SourceDirectoryIs			"The source directories is '%s'." ;# NEW
::fsbox::mc::NewName							"New name" ;# NEW

::fsbox::mc::ReallyMove(file,w)			"¿Realmente desea mover el archivo '%s' a la papelera?"
::fsbox::mc::ReallyMove(file,r)			"¿Realmente desea mover el archivo de solo lectura '%s' a la papelera?"
::fsbox::mc::ReallyMove(folder,w)		"¿Realmente desea mover la carpeta '%s' a la papelera?"
::fsbox::mc::ReallyMove(folder,r)		"¿Realmente desea mover la carpeta de solo lectura '%s' a la papelera?"
::fsbox::mc::ReallyDelete(file,w)		"¿Realmente desea borrar el archivo '%s'? Esta operación no puede revertirse."
::fsbox::mc::ReallyDelete(file,r)		"¿Realmente desea borrar el archivo de solo lectura '%s'? Esta operación no puede revertirse."
::fsbox::mc::ReallyDelete(link,w)		"¿Realmente desea borrar el vúnculo a '%s'?"
::fsbox::mc::ReallyDelete(link,r)		"¿Realmente desea borrar el vúnculo a '%s'?"
::fsbox::mc::ReallyDelete(folder,w)		"¿Realmente desea borrar la carpeta '%s'? Esta operación no puede revertirse."
::fsbox::mc::ReallyDelete(folder,r)		"¿Realmente desea borrar la carpeta de solo lectura '%s'? Esta operación no puede revertirse."

::fsbox::mc::ErrorRenaming(folder)		"No se puede cambiar el nombre de la carpeta '%old' a '%new': permiso denegado."
::fsbox::mc::ErrorRenaming(file)			"Error cambiando de nombre el archivo '%old' a '%new': permiso denegado."

::fsbox::mc::Cannot(delete)				"No se puede borrar el archivo '%s'."
::fsbox::mc::Cannot(rename)				"No se puede renombrar el archivo '%s'."
::fsbox::mc::Cannot(move)					"Cannot move file '%s'." ;# NEW
::fsbox::mc::Cannot(overwrite)			"No se puede sobrescribir el archivo '%s'." 

::fsbox::mc::DropAction(move)				"Move Here" ;# NEW
::fsbox::mc::DropAction(copy)				"Copy Here" ;# NEW
::fsbox::mc::DropAction(link)				"Link Here" ;# NEW

### toolbar ############################################################
::toolbar::mc::Toolbar		"Barra de herramientas"
::toolbar::mc::Orientation	"Orientación"
::toolbar::mc::Alignment	"Alineación"
::toolbar::mc::IconSize		"Tamaño de íconos"

::toolbar::mc::Default		"Predeterminado"
::toolbar::mc::Small			"Pequeño"
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
::country::mc::Afghanistan											"Afganistán"
::country::mc::Netherlands_Antilles								"Antillas Holandesas"
::country::mc::Anguilla												"Anguila"
::country::mc::Aboard_Aircraft									"A bordo de un avión"
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
::country::mc::Antarctica											"Antártida"
::country::mc::French_Southern_Territories					"Territorios franceses del sur"
::country::mc::Australia											"Australia"
::country::mc::Austria												"Austria"
::country::mc::Azerbaijan											"Azerbaijan"
::country::mc::Bahamas												"Bahamas"
::country::mc::Bangladesh											"Bangladesh"
::country::mc::Barbados												"Barbados"
::country::mc::Basque												"Vascongada"
::country::mc::Burundi												"Burundi"
::country::mc::Belgium												"Bélgica"
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
::country::mc::Central_African_Republic						"República Centroafricana"
::country::mc::Cambodia												"Camboya"
::country::mc::Canada												"Canadá"
::country::mc::Catalonia											"Cataluña"
::country::mc::Cayman_Islands										"Islas Caimán"
::country::mc::Cocos_Islands										"Islas Cocos"
::country::mc::Congo													"Congo (Brazzaville)"
::country::mc::Chad													"Chad"
::country::mc::Chile													"Chile"
::country::mc::China													"China"
::country::mc::Ivory_Coast											"Costa de Marfil"
::country::mc::Cameroon												"Camerún"
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
::country::mc::Czech_Republic										"República Checa"
::country::mc::Denmark												"Dinamarca"
::country::mc::Djibouti												"Djibouti"
::country::mc::Dominica												"Dominica"
::country::mc::Dominican_Republic								"República Dominicana"
::country::mc::Ecuador												"Ecuador"
::country::mc::Egypt													"Egipto"
::country::mc::England												"Inglaterra"
::country::mc::Eritrea												"Etiopía"
::country::mc::El_Salvador											"El Salvador"
::country::mc::Western_Sahara										"Sahara Occidental"
::country::mc::Spain													"España"
::country::mc::Estonia												"Estonia"
::country::mc::Ethiopia												"Etiopía"
::country::mc::Faroe_Islands										"Islas Faroe"
::country::mc::Fiji													"Fidji"
::country::mc::Finland												"Finlandia"
::country::mc::Falkland_Islands									"Islas Malvinas"
::country::mc::France												"Francia"
::country::mc::West_Germany										"Alemania Occidental"
::country::mc::Micronesia											"Micronesia"
::country::mc::Gabon													"Gabón"
::country::mc::Gambia												"Gambia"
::country::mc::Great_Britain										"Gran Bretaña"
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
::country::mc::Haiti													"Haití"
::country::mc::Hong_Kong											"Hong Kong"
::country::mc::Heard_Island_and_McDonald_Islands			"Isla Heard e Islas McDonald"
::country::mc::Honduras												"Honduras"
::country::mc::Hungary												"Hungría"
::country::mc::Isle_of_Man											"Isla de Man"
::country::mc::Indonesia											"Indonesia"
::country::mc::India													"India"
::country::mc::British_Indian_Ocean_Territory				"Territorio Británico del Océano Índico"
::country::mc::Iran													"Irán"
::country::mc::Ireland												"Irlanda"
::country::mc::Iraq													"Irak"
::country::mc::Iceland												"Islandia"
::country::mc::Israel												"Israel"
::country::mc::Italy													"Italia"
::country::mc::British_Virgin_Islands							"Islas Vírgenes Británicas"
::country::mc::Jamaica												"Jamaica"
::country::mc::Jersey												"Jersey"
::country::mc::Jordan												"Jordania"
::country::mc::Japan													"Japón"
::country::mc::Kazakhstan											"Kazajstán"
::country::mc::Kenya													"Kenia"
::country::mc::Kosovo												"Kosovo"
::country::mc::Kyrgyzstan											"Kirguizistán"
::country::mc::Kiribati												"Kiribati"
::country::mc::South_Korea											"Corea del Sur"
::country::mc::Saudi_Arabia										"Arabia Saudita"
::country::mc::Kuwait												"Kuwait"
::country::mc::Laos													"Laos"
::country::mc::Latvia												"Letonia"
::country::mc::Libya													"Libia"
::country::mc::Liberia												"Liberia"
::country::mc::Saint_Lucia											"Santa Lucía"
::country::mc::Lesotho												"Lesotho"
::country::mc::Lebanon												"Líbano"
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
::country::mc::Netherlands											"Países Bajos"
::country::mc::Nepal													"Nepal"
::country::mc::The_Internet										"Internet"
::country::mc::Norfolk_Island										"Isla Norfolk"
::country::mc::Nigeria												"Nigeria"
::country::mc::Niger													"Níger"
::country::mc::Northern_Ireland									"Irlanda del Norte"
::country::mc::Niue													"Niue"
::country::mc::Norway												"Noruega"
::country::mc::Nauru													"Nauru"
::country::mc::New_Zealand											"Nueva Zelanda"
::country::mc::Oman													"Omán"
::country::mc::Pakistan												"Pakistán"
::country::mc::Panama												"Panamá"
::country::mc::Paraguay												"Paraguay"
::country::mc::Pitcairn_Islands									"Islas Pitcairn"
::country::mc::Peru													"Perú"
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
::country::mc::Reunion												"Reunión"
::country::mc::Romania												"Rumania"
::country::mc::South_Africa										"Sudáfrica"
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
::country::mc::Somalia												"Somalía"
::country::mc::Aboard_Spacecraft									"A bordo del transbordador espacial"
::country::mc::Saint_Pierre_and_Miquelon						"Saint Pierre y Miquelon"
::country::mc::Serbia												"Serbia"
::country::mc::Sri_Lanka											"Sri Lanka"
::country::mc::Sao_Tome_and_Principe							"Sao Tome y Príncipe"
::country::mc::Sudan													"Sudán"
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
::country::mc::Tibet													"Tíbet"
::country::mc::Tajikistan											"Tajikistán"
::country::mc::Tokelau												"Tokelau"
::country::mc::Turkmenistan										"Turkmenistán"
::country::mc::Timor_Leste											"Timor Leste"
::country::mc::Togo													"Togo"
::country::mc::Chinese_Taipei										"Taiwán"
::country::mc::Trinidad_and_Tobago								"Trinidad y Tobago"
::country::mc::Tunisia												"Túnez"
::country::mc::Turkey												"Turquía"
::country::mc::Tuvalu												"Tuvalu"
::country::mc::United_Arab_Emirates								"Emiratos Árabes Unidos"
::country::mc::Uganda												"Uganda"
::country::mc::Ukraine												"Ucrania"
::country::mc::United_States_Minor_Outlying_Islands		"Islas Remotas Menores de los Estados Unidos"
::country::mc::Unknown												"(Desconocido)"
::country::mc::Soviet_Union										"Unión Soviética"
::country::mc::Uruguay												"Uruguay"
::country::mc::United_States_of_America						"Estados Unidos de América"
::country::mc::Uzbekistan											"Uzbekistán"
::country::mc::Vanuatu												"Vanuatu"
::country::mc::Vatican												"Vaticano"
::country::mc::Venezuela											"Venezuela"
::country::mc::Vietnam												"Vietnam"
::country::mc::Saint_Vincent_and_the_Grenadines				"Saint Vincent y las Granadinas"
::country::mc::US_Virgin_Islands									"Islas Vírgenes Norteamericanas"
::country::mc::Wallis_and_Futuna									"Wallis y Futuna"
::country::mc::Wales													"Gales"
::country::mc::Yemen													"Yemen"
::country::mc::Yugoslavia											"Yugoslavia"
::country::mc::Zambia												"Zambia"
::country::mc::Zanzibar												"Zanzíbar"
::country::mc::Zimbabwe												"Zimbabwe"
::country::mc::Mixed_Team											"Equipo mixto"

::country::mc::Africa_North										"Africa, Norte"
::country::mc::Africa_Sub_Saharan								"Africa, Sub-Sahariana"
::country::mc::America_Caribbean									"América, Caribe"
::country::mc::America_Central									"América Central"
::country::mc::America_North										"América, Norte"
::country::mc::America_South										"América, Sur"
::country::mc::Antarctic											"Antártida"
::country::mc::Asia_East											"Asia, Este"
::country::mc::Asia_South_South_East							"Asia, Sud-Sudeste"
::country::mc::Asia_West_Central									"Asia, Oeste-Central"
::country::mc::Europe												"Europa"
::country::mc::Europe_East											"Europa, Este"
::country::mc::Oceania												"Oceanía"
::country::mc::Stateless											"Sin estado"

### Languages ##########################################################
::encoding::mc::Lang(FI)	"Fide"
::encoding::mc::Lang(af)	"Afrikaans"
::encoding::mc::Lang(ar)	"Árabe"
::encoding::mc::Lang(ast)	"Leonese"
::encoding::mc::Lang(az)	"Azerbaijani"
::encoding::mc::Lang(bat)	"Báltico"
::encoding::mc::Lang(be)	"Bielorruso"
::encoding::mc::Lang(bg)	"Búlgaro"
::encoding::mc::Lang(br)	"Bretón"
::encoding::mc::Lang(bs)	"Bosnio"
::encoding::mc::Lang(ca)	"Catalán"
::encoding::mc::Lang(cs)	"Checo"
::encoding::mc::Lang(cy)	"Galés"
::encoding::mc::Lang(da)	"Danés"
::encoding::mc::Lang(de)	"Alemán"
::encoding::mc::Lang(de+)	"Alemán (reformado)"
::encoding::mc::Lang(el)	"Griego"
::encoding::mc::Lang(en)	"Inglés"
::encoding::mc::Lang(eo)	"Esperanto"
::encoding::mc::Lang(es)	"Español"
::encoding::mc::Lang(et)	"Estonio"
::encoding::mc::Lang(eu)	"Vasco"
::encoding::mc::Lang(fi)	"Finés"
::encoding::mc::Lang(fo)	"Faroés"
::encoding::mc::Lang(fr)	"Francés"
::encoding::mc::Lang(fy)	"Frisón"
::encoding::mc::Lang(ga)	"Irlandés"
::encoding::mc::Lang(gd)	"Escocés"
::encoding::mc::Lang(gl)	"Gallego"
::encoding::mc::Lang(he)	"Hebreo"
::encoding::mc::Lang(hi)	"Hindi"
::encoding::mc::Lang(hr)	"Croata"
::encoding::mc::Lang(hu)	"Húngaro"
::encoding::mc::Lang(hy)	"Armenio"
::encoding::mc::Lang(ia)	"Interlingua"
::encoding::mc::Lang(is)	"Islandés"
::encoding::mc::Lang(it)	"Italiano"
::encoding::mc::Lang(iu)	"Inuktitut"
::encoding::mc::Lang(ja)	"Japonés"
::encoding::mc::Lang(ka)	"Georgiano"
::encoding::mc::Lang(kk)	"Kazakho"
::encoding::mc::Lang(kl)	"Groenlandés"
::encoding::mc::Lang(ko)	"Coreano"
::encoding::mc::Lang(ku)	"Kurdo"
::encoding::mc::Lang(ky)	"Kirghiz"
::encoding::mc::Lang(la)	"Latin"
::encoding::mc::Lang(lb)	"Luxemburgués"
::encoding::mc::Lang(lt)	"Lituano"
::encoding::mc::Lang(lv)	"Letón"
::encoding::mc::Lang(mk)	"Macedonio"
::encoding::mc::Lang(mo)	"Moldavo"
::encoding::mc::Lang(ms)	"Malayo"
::encoding::mc::Lang(mt)	"Maltés"
::encoding::mc::Lang(nl)	"Holandés"
::encoding::mc::Lang(no)	"Noruego"
::encoding::mc::Lang(oc)	"Occitano"
::encoding::mc::Lang(pl)	"Polaco"
::encoding::mc::Lang(pt)	"Portugués"
::encoding::mc::Lang(rm)	"Romaní"
::encoding::mc::Lang(ro)	"Rumano"
::encoding::mc::Lang(ru)	"Ruso"
::encoding::mc::Lang(se)	"Sami"
::encoding::mc::Lang(sk)	"Eslocavo"
::encoding::mc::Lang(sl)	"Esloveno"
::encoding::mc::Lang(sq)	"Albanés"
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
::encoding::mc::Lang(wen)	"Sorbio"
::encoding::mc::Lang(hsb)	"Alto Sorbio"
::encoding::mc::Lang(dsb)	"Bajo Sorbio"
::encoding::mc::Lang(zh)	"Chino"

::encoding::mc::Font(hi)	"Devanagari"

### Calendar ###########################################################
::calendar::mc::OneMonthForward	"Avanzar un mes (Shift \u2192)"
::calendar::mc::OneMonthBackward	"Retroceder un mes (Shift \u2190)"
::calendar::mc::OneYearForward	"Avanzar un año (Ctrl \u2192)"
::calendar::mc::OneYearBackward	"Retroceder un año (Ctrl \u2190)"

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
::calendar::mc::WeekdayName(3)	"Miércoles"
::calendar::mc::WeekdayName(4)	"Jueves"
::calendar::mc::WeekdayName(5)	"Viernes"
::calendar::mc::WeekdayName(6)	"Sábado"

### remote #############################################################
::remote::mc::PostponedMessage "La apertura de la base \"%s\" se pospondrá hasta que concluya la operación en curso."

# vi:set ts=3 sw=3:
