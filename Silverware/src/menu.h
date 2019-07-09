typedef struct Menu{
		char dir;
		float PID_value;
		char menu_class;      //0:main_menu   1:PID_menu   2:motor_menu
		char menu_index;
		struct Menu *next;
		struct Menu *prior;
	//	struct Menu *target_menu;
}Menu_Node,*Menu_List;

Menu_List CreateDbCcLinkList(char length,char list_class);
