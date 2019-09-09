void onMain() {

	pointer pC = new HClassTest;
	pC->Name1 = "Hello world";
	pC->Testprop1 = 32;

	print(pC->Name1);
	print(pC->Testprop1);

	Recurse(0);

	return;
}

void Recurse(int a) {
	a++;
	if(a<=10) {
		print(a);
		Recurse(a);
	}
}