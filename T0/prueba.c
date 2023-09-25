void batiplaylist_union(Batiplaylist* playlist1, Batiplaylist* playlist2) {
	Batiplaylist* current = playlist1;
	while (current->next != NULL) {
		current = current->next;
	}
	if (current->cancion != NULL) {
		current->next = playlist2;
	}
	else {
		current->cancion = playlist2->cancion;
		current->next = playlist2->next;
	}
	// if (current == NULL) {
	//	current = playlist2;
	// }
	while (current->next != NULL) {
		current->id_playlist = playlist1->id_playlist;
		current = current->next;
	}
	if (current->next == NULL && current->cancion != NULL) {
		current->id_playlist = playlist1->id_playlist;
	}
	playlist2->id_playlist = playlist1->id_playlist;
}